import QtQuick 2.0
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1


Item {
    id:msgItem;
    width:920;
    height: 180;

    signal sendMsg(string sendtext);
    signal takeColor(string colorname);
    signal taketextColor(string colorname);
    property string bgcolorname: "#23190f"
    property string textcolorname: "#dce6f0"
    function setcolor(color1,color2){
        bgcolorname=color1;
        textcolorname=color2;
    }
    Rectangle{
        id:bgRectangle;
        anchors.fill:parent;
        color: bgcolorname;
    }

    Component{
        id:onePage;
        Item {
            height: 160;
            width: 160;
            id: name

            Image {
                id: test1;
                source: pagepath;
            }
            Text {
                id: imgename;
                anchors.horizontalCenter: parent.horizontalCenter;
                anchors.top: parent.bottom;
                font.pixelSize: 30;
                color: textcolorname;
                text: classifyname;
            }
            MouseArea{
                anchors.fill:parent;
                onClicked: {
                    sendMsg(imgename.text);
                    console.log(imgename.text);
                }
            }
        }
    }

    ListModel{
        id:pagelistmodel;
        ListElement{
            pagepath:"qrc:/imgeList.jpg";
            classifyname:"aaa";
        }
        ListElement{
            pagepath:"qrc:/imgeList.jpg";
            classifyname:"aaa";
        }
        ListElement{
            pagepath:"qrc:/imgeList.jpg";
            classifyname:"456";
        }
        ListElement{
            pagepath:"qrc:/imgeList.jpg";
            classifyname:"123";
        }
        ListElement{
            pagepath:"qrc:/imgeList.jpg";
            classifyname:"aaa";
        }
        ListElement{
            pagepath:"qrc:/imgeList.jpg";
            classifyname:"aaa";
        }
        ListElement{
            pagepath:"qrc:/imgeList.jpg";
            classifyname:"aaa";
        }
        ListElement{
            pagepath:"qrc:/imgeList.jpg";
            classifyname:"aaa";
        }
        ListElement{
            pagepath:"qrc:/imgeList.jpg";
            classifyname:"aaa";
        }
        ListElement{
            pagepath:"qrc:/imgeList.jpg";
            classifyname:"aaa";
        }
    }

    ListView{
        id:mainListview;
        width: parent.width;
        height: parent.height;
        orientation: ListView.Horizontal;//列表移动方向为水平
        delegate:onePage
        model: pagelistmodel;

    }

    onTakeColor: {
        bgRectangle.color=colorname;
    }
    onTaketextColor: {
        textcolorname=colorname;
    }

}
