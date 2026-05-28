"""
NetEase Cloud Music scraper — searches for songs, downloads MP3s,
and stores metadata in MySQL. Driven by IPC files from the Qt C++ client.
"""
import json
import logging
import re
import signal
import sys
import time
from pathlib import Path

import pymysql
import requests
from selenium import webdriver
from selenium.webdriver.common.by import By
from selenium.webdriver.support import expected_conditions as EC
from selenium.webdriver.support.ui import WebDriverWait

# ---------------------------------------------------------------------------
# Configuration — adjust paths / credentials for your environment
# ---------------------------------------------------------------------------
CONFIG = {
    "db": {
        "host": "127.0.0.1",
        "port": 3306,
        "database": "music_info",
        "charset": "utf8",
        "user": "root",
        "passwd": "111111",
    },
    "paths": {
        "find_file": Path("C:/Users/33746/Desktop/Find.txt"),
        "wantfind_file": Path("C:/Users/33746/Desktop/WantFind.txt"),
        "download_dir": Path("C:/Users/33746/Desktop/ServerFindMusic"),
        "cookies_file": Path("cookies.txt"),
    },
    "urls": {
        "home": "https://music.163.com",
        "search": "https://music.163.com/#/search/m/?s=",
        "media_outer": "http://music.163.com/song/media/outer/url?id=",
    },
    "headers": {
        "User-Agent": (
            "Mozilla/5.0 (Windows NT 10.0; Win64; x64) "
            "AppleWebKit/537.36 (KHTML, like Gecko) "
            "Chrome/125.0.0.0 Safari/537.36 Edg/125.0.0.0"
        ),
    },
}

logger = logging.getLogger("music_scraper")

# ---------------------------------------------------------------------------
# Helpers
# ---------------------------------------------------------------------------
def _extract_song_id(url: str) -> str:
    """Return the numeric song id from a URL like '...song?id=1234567'."""
    return url.rsplit("=", 1)[-1]


def _sanitise_filename(name: str) -> str:
    """Strip characters that are illegal in Windows file names."""
    return re.sub(r'[\\/*?:"<>|]', "_", name)


# ---------------------------------------------------------------------------
# Database
# ---------------------------------------------------------------------------
def _db_connection():
    return pymysql.connect(**CONFIG["db"])


def insert_find_record(music_id: str, song_name: str, username: str) -> None:
    sql = "INSERT INTO find_list (findlist_name, findlist_musicid, user_name) VALUES (%s, %s, %s)"
    with _db_connection() as conn:
        with conn.cursor() as cur:
            cur.execute(sql, (song_name, music_id, username))
        conn.commit()


def insert_musicinfo_record(music_id: str, song_name: str, path: str) -> None:
    sql = "INSERT INTO musicinfo_list (findlist_musicid, music_name, music_path) VALUES (%s, %s, %s)"
    with _db_connection() as conn:
        with conn.cursor() as cur:
            cur.execute(sql, (music_id, song_name, path))
        conn.commit()


# ---------------------------------------------------------------------------
# IPC  (communicates with the Qt C++ client via desktop text files)
# ---------------------------------------------------------------------------
def _read_ipc_parts() -> list[str]:
    """Read the IPC file and return its '/' -delimited parts."""
    raw = CONFIG["paths"]["find_file"].read_text(encoding="gbk")
    return raw.strip().split("/")


def take_username() -> str:
    return _read_ipc_parts()[0]


def take_music_name() -> str:
    return _read_ipc_parts()[1]


def signal_finished() -> None:
    CONFIG["paths"]["find_file"].write_text("finish", encoding="utf8")


def append_wantfind(name: str) -> None:
    with CONFIG["paths"]["wantfind_file"].open("a", encoding="utf8") as f:
        f.write(name + "\n")


# ---------------------------------------------------------------------------
# Cookies
# ---------------------------------------------------------------------------
def load_cookies(driver: webdriver.Edge) -> bool:
    """Load saved cookies into the browser so we can skip manual login.

    Returns True if cookies were loaded, False otherwise.
    """
    cookies_path = CONFIG["paths"]["cookies_file"]
    if not cookies_path.exists():
        logger.warning("Cookies file %s not found — manual login may be needed.", cookies_path)
        return False

    cookies = json.loads(cookies_path.read_text(encoding="utf8"))
    for cookie in cookies:
        driver.add_cookie(cookie)
    logger.info("Loaded %d cookies from %s", len(cookies), cookies_path)
    return True


# ---------------------------------------------------------------------------
# Scraper
# ---------------------------------------------------------------------------
class MusicScraper:
    """Encapsulates the Selenium-based music search & download workflow."""

    def __init__(self) -> None:
        self.driver = webdriver.Edge()
        self.wait = WebDriverWait(self.driver, 15)

    # -- navigation ----------------------------------------------------------
    def start(self) -> None:
        logger.info("Navigating to %s", CONFIG["urls"]["home"])
        self.driver.get(CONFIG["urls"]["home"])
        time.sleep(3)  # let the landing page settle before cookie injection

        load_cookies(self.driver)

        self.driver.refresh()
        self.driver.get(CONFIG["urls"]["search"])
        logger.info("Ready — waiting for search queries.")

    # -- search a single song -------------------------------------------------
    def search(self, song_name: str) -> None:
        url = CONFIG["urls"]["search"] + song_name
        logger.info("Searching: %s", song_name)
        self.driver.get(url)

        # switch into the embedded iframe that holds search results
        iframe = self.wait.until(
            EC.presence_of_element_located((By.XPATH, "//iframe[@id='g_iframe']"))
        )
        self.driver.switch_to.frame(iframe)

    # -- extract results & download -------------------------------------------
    def scrape_and_download(self, username: str) -> int:
        """Scrape song links and <b> titles from the current iframe,
        download each MP3, and persist DB records.  Returns count of
        successful downloads."""
        links = self.driver.find_elements(By.TAG_NAME, "a")
        bold_tags = self.driver.find_elements(By.TAG_NAME, "b")

        # write song names to WantFind.txt for the Qt client
        for b in bold_tags:
            append_wantfind(b.text)

        song_pattern = re.compile(r"https://music\.163\.com/song.*")

        count = 0
        idx = 0
        for a in links:
            href = a.get_attribute("href")
            if not href:
                continue
            match = song_pattern.search(href)
            if not match:
                continue
            if idx >= len(bold_tags):
                break

            music_id = _extract_song_id(match.group())
            song_name = bold_tags[idx].text
            idx += 1

            # download MP3
            mp3_url = CONFIG["urls"]["media_outer"] + music_id
            try:
                data = requests.get(mp3_url, headers=CONFIG["headers"], timeout=30).content
            except requests.RequestException:
                logger.exception("Download failed for id=%s name=%s", music_id, song_name)
                continue

            safe_name = _sanitise_filename(song_name)
            file_path = CONFIG["paths"]["download_dir"] / f"{safe_name}.mp3"

            try:
                file_path.write_bytes(data)
            except OSError:
                logger.exception("Cannot write %s", file_path)
                continue

            # persist to DB
            try:
                insert_find_record(music_id, song_name, username)
                insert_musicinfo_record(music_id, song_name, str(file_path))
            except pymysql.MySQLError:
                logger.exception("DB insert failed for %s", song_name)
                # file already saved — continue
            count += 1

        return count

    # -- cleanup --------------------------------------------------------------
    def quit(self) -> None:
        self.driver.quit()
        logger.info("Browser closed.")


# ---------------------------------------------------------------------------
# Main loop
# ---------------------------------------------------------------------------
RUNNING = True


def _handle_shutdown(signum, frame):
    global RUNNING
    logger.info("Received signal %d — shutting down after current iteration.", signum)
    RUNNING = False


def main() -> None:
    logging.basicConfig(
        level=logging.INFO,
        format="%(asctime)s [%(levelname)s] %(message)s",
        datefmt="%H:%M:%S",
    )

    signal.signal(signal.SIGINT, _handle_shutdown)
    signal.signal(signal.SIGTERM, _handle_shutdown)

    # ensure download directory exists
    CONFIG["paths"]["download_dir"].mkdir(parents=True, exist_ok=True)

    scraper = MusicScraper()
    try:
        scraper.start()

        while RUNNING:
            song = take_music_name()
            logger.info("Requested song: %s", song)

            scraper.search(song)
            username = take_username()
            downloaded = scraper.scrape_and_download(username)

            logger.info("Downloaded %d tracks for '%s'", downloaded, song)
            signal_finished()
    finally:
        scraper.quit()


if __name__ == "__main__":
    main()
