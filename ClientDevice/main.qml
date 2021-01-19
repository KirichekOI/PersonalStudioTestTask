import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

ApplicationWindow {
    id: appWindow
    visible: true
    width: 640
    height: 480
    title: qsTr("Multimeter client")

    Connections {
        target: clientSocket

        onPrintInfoMsg: {
            infoTextArea.append(msg);
        }

        onInitQmlModels: {
            for (var i = 0; i < cmdType.length; ++i)
                cmdTypeModel.append({text: cmdType[i]})
            for (i = 0; i < channelNameList.length; ++i)
                channelNumsTypeModel.append({text: channelNameList[i]})
            for (i = 0; i < rangeNameList.length; ++i)
                rangeNumsTypeModel.append({text: rangeNameList[i]})
        }
    }

    Button {
        id: btnConnectToServer
        x: 487
        text: qsTr("Connect to server")
        font.pointSize: 12
        wheelEnabled: false
        anchors.right: parent.right
        anchors.rightMargin: 16
        anchors.top: parent.top
        anchors.topMargin: 45
        onClicked: {
            clientSocket.connectToServer();
        }
    }

    Button {
        id: btnSendCmd
        height: 40
        text: qsTr("Send command")
        font.pointSize: 12
        anchors.right: btnConnectToServer.right
        anchors.rightMargin: 0
        anchors.left: btnConnectToServer.left
        anchors.leftMargin: 0
        anchors.top: btnConnectToServer.bottom
        anchors.topMargin: 11
        onClicked: {
            var params = [];
            params.push(cbChannelNums.currentText);
            if (cbRangeNums.enabled)
                params.push(cbRangeNums.currentText);
            clientSocket.slotSendToServer(cbCmdType.currentText, params);
        }
    }

    ComboBox {
        id: cbCmdType
        model: ListModel {
            id: cmdTypeModel
        }
        font.pointSize: 12
        anchors.left: parent.left
        anchors.leftMargin: 16
        anchors.bottom: btnConnectToServer.bottom
        anchors.bottomMargin: 0
        anchors.top: btnConnectToServer.top
        anchors.topMargin: 0
        onActivated: {
            if (cbCmdType.currentIndex == 1) {
                cbRangeNums.enabled = true;
            }
            else {
                cbRangeNums.enabled = false;
            }
        }
    }

    ComboBox {
        id: cbChannelNums
        model: ListModel {
            id: channelNumsTypeModel
        }
        font.pointSize: 12
        anchors.left: cbCmdType.right
        anchors.leftMargin: 15
        anchors.bottom: btnConnectToServer.bottom
        anchors.bottomMargin: 0
        anchors.top: btnConnectToServer.top
        anchors.topMargin: 0
    }

    ComboBox {
        id: cbRangeNums
        model: ListModel {
            id: rangeNumsTypeModel
        }
        font.pointSize: 12
        anchors.left: cbChannelNums.right
        anchors.leftMargin: 15
        anchors.bottom: btnConnectToServer.bottom
        anchors.bottomMargin: 0
        anchors.top: btnConnectToServer.top
        anchors.topMargin: 0
    }

    Text {
        id: elementChannelNums
        y: 20
        text: qsTr("Channel nums")
        anchors.left: cbChannelNums.left
        anchors.leftMargin: 0
        anchors.bottom: cbChannelNums.top
        anchors.bottomMargin: 10
        font.pixelSize: 12
    }

    Text {
        id: elementCmdType
        y: 20
        text: qsTr("Command type")
        anchors.bottom: cbCmdType.top
        anchors.bottomMargin: 10
        anchors.left: cbCmdType.left
        anchors.leftMargin: 0
        font.pixelSize: 12
    }

    Text {
        id: elementRangeNums
        y: 20
        text: qsTr("Range nums")
        anchors.left: cbRangeNums.left
        anchors.leftMargin: 0
        anchors.bottom: cbRangeNums.top
        anchors.bottomMargin: 10
        font.pixelSize: 12
    }

    Rectangle {
        id: textAreaFrame
        parent: appWindow
        border.color: "blue"
        anchors.right: parent.right
        anchors.rightMargin: 16
        anchors.left: parent.left
        anchors.leftMargin: 16
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 14
        anchors.top: btnSendCmd.bottom
        anchors.topMargin: 61
        ScrollView {
            id: scrollView
            parent: textAreaFrame
            anchors.fill: parent
            ScrollBar.vertical.policy: ScrollBar.AsNeeded
            ScrollBar.horizontal.policy: ScrollBar.AsNeeded
            TextArea {
                id: infoTextArea
                readOnly: true
                font.pointSize: 12
                anchors.fill: parent
            }
        }
    }
}
