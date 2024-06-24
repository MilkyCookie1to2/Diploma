import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls
import QtMultimedia
import QtQuick.Dialogs
import QtQuick.Layouts
import Qt5Compat.GraphicalEffects
import QtQuick.Shapes

Window {
    width: 640
    height: 480
    visible: true
    visibility: "FullScreen"
    title: qsTr("Hello World")

    Rectangle {
        id: rootRec
        width: parent.width
        height: parent.height
        color: "#121318"

        Rectangle{
            id: imageRect
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            width: 523
            height: parent.height-(0.05*parent.height)

            color: "transparent"

            Image{
                id: opencvImage
                anchors.fill: parent
                fillMode: Image.PreserveAspectFit
                property bool counter: false
                visible: true
                source: "image://live/image"
                asynchronous: false
                cache: false

                function reload()
                {

                    counter = !counter
                    source = "image://live/image?id=" + counter
                }

            }
        }

        Rectangle {
            id: leftBackground
            x: -(parent.height - imageRect.height)/2
            y: 0
            radius: (parent.height - imageRect.height)/2
            width: (parent.width-imageRect.width)/2
            height: parent.height
            color: "#2f3036"

            Rectangle {
                id: printRec
                x: leftBackground.width/2 - 50
                y: 10
                width: 100
                height: 60
                radius: 30
                color: "#dbe1ff"

                RowLayout {
                    anchors.fill: parent
                    Layout.alignment: Qt.AlignCenter

                    Rectangle {
                        id: heart
                        width: 40
                        height: 40
                        color: "transparent"
                        Layout.alignment: Qt.AlignCenter

                        Image {
                            id: icon_heart
                            anchors.fill: parent
                            fillMode: Image.PreserveAspectFit

                            source: "qrc:/heart_icon.svg"
                        }
                    }

                    Text {
                        id: countLifes
                        Layout.alignment: Qt.AlignVCenter
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        font.family: "Century Gothic"
                        font.bold: true
                        font.pixelSize: 30
                        color: "#01174b"
                    }
                }

            }

            Button{
                id: leftButton
                x: leftBackground.width/2 - 2 - 60
                y: leftBackground.height/2 - 60
                autoRepeat: true

                onPressed: {
                    inputPlayer.inputProcessor(1);
                }

                background: Shape {
                    ShapePath {
                        fillColor: "#dbe1ff"
                        strokeWidth: 0
                        strokeColor: "transparent"
                        startX: 0; startY: 0

                        PathArc {
                            relativeX: 0; y: 120
                            radiusX: 60; radiusY: 60
                        }
                    }

                    Image {
                        anchors.fill: parent
                        fillMode: Image.PreserveAspectFit

                        source: "qrc:/arrow_icon.svg"
                    }

                    transform: Rotation { origin.x: 30; origin.y: 60; angle: 180 }
                }

            }

            Button{
                id: rightButton
                x: leftBackground.width/2 + 2
                y: leftBackground.height/2 - 60
                autoRepeat: true

                onPressed: {
                    inputPlayer.inputProcessor(2);
                }

                background: Shape {
                    ShapePath {
                        fillColor: "#dbe1ff"
                        strokeWidth: 0
                        strokeColor: "transparent"
                        startX: 0; startY: 0

                        PathArc {
                            relativeX: 0; y: 120
                            radiusX: 60; radiusY: 60
                        }

                    }

                    Image {
                        anchors.fill: parent
                        fillMode: Image.PreserveAspectFit

                        source: "qrc:/arrow_icon.svg"
                    }
                }
            }
        }

        Rectangle {
            id: rightBackground
            radius: leftBackground.radius
            width: (parent.width-imageRect.width)/2
            height: parent.height
            x: parent.width - rightBackground.width + (parent.height - imageRect.height)/2
            y: 0
            color: "#2f3036"

            Button{
                id: pshButton
                x: rightBackground.width/2 - 60
                y: rightBackground.height/2 - 60
                autoRepeat: true

                background: Shape {
                    ShapePath {
                        fillColor: "#dbe1ff"
                        strokeWidth: 0
                        strokeColor: "transparent"
                        startX: 60; startY: 0

                        PathArc {
                            x: 60; y: 120
                            radiusX: 60; radiusY: 60
                        }

                        PathArc {
                            x: 60; y: 0
                            radiusX: 60; radiusY: 60
                        }
                    }
                }

                contentItem: Text {
                    text: qsTr("PUSH")
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.family: "Century Gothic"
                    font.bold: true
                    font.pixelSize: 30
                    color: "#01174b"
                }

                onPressed: {
                     inputPlayer.inputProcessor(3);
                }
            }

            Button {
                x: rightBackground.width/2 - 50
                y: 10
                width: 100
                height: 60

                background: Rectangle {
                    radius: 30
                    color: "#dbe1ff"

                    Image {
                        anchors.fill: parent
                        fillMode: Image.PreserveAspectFit
                        anchors.margins: 5

                        source: "qrc:/heal_icon.svg"
                    }
                }

                onPressed: {
                     inputPlayer.inputProcessor(4);
                }
            }
        }

    }

    Connections{
        target: streamProvider

        function onImageChanged()
        {
            opencvImage.reload()
        }
    }

    function showError(errorMessage)
    {
        message.text = "Ошибка"
        message.informativeText = errorMessage
        message.open()
    }

    function showErrorWiFi()
    {
        console.log("ERROR_WIFI");
        message.text = "Ошибка подключения"
        message.informativeText = "Для работы приложения требуется подключение через Wi-Fi. Включите Wi-Fi и повторите попытку"
        message.buttons = MessageDialog.Close
        message.open()
    }

    function setCountLifes(number) {
        countLifes.text = number
    }

    MessageDialog {
        id: message
        buttons: MessageDialog.Retry | MessageDialog.Close

        onButtonClicked: function (button, role) {
            switch (button) {
            case MessageDialog.Retry:
                connectionManager.checkWiFiConnection();
                break;
            case MessageDialog.Close:
                Qt.quit();
                break;
            }
        }

    }

    Dialog {
        id: emptyLifes
        anchors.centerIn: parent
        width: 320
        height: 240
        closePolicy: Popup.CloseOnEscape

        background: Rectangle {
            radius: 30
            color: "#121318"
        }

        ColumnLayout {
            anchors.fill: parent

            Text {
                id: titleEmpty
                anchors.topMargin: 3
                text: "Осталось 0 жизней"
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
                font.bold: true
                font.family: "Century Gothic"
                font.pointSize: 24
                color: "white"
            }

            Text {
                id: textEmpty
                anchors.topMargin: 2
                width: 120
                text: "Необходимо восстановить\nжизни"
                wrapMode: Text.WordWrap
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                Layout.alignment: Qt.AlignHCenter
                font.bold: true
                font.family: "Century Gothic"
                font.pointSize: 18
                color: "white"
            }

            Rectangle {
                id: buttonEmpty
                width: 230
                height: 40
                radius: 20
                color: "#dbe1ff"
                anchors.bottomMargin: 3
                Layout.alignment: Qt.AlignHCenter | Qt.AlignBottom

                Text {
                    anchors.fill: parent
                    anchors.margins: 1
                    text: qsTr("Восстановить жизни")
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.family: "Century Gothic"
                    font.bold: true
                    font.pixelSize: 18
                    color: "#01174b"
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        inputPlayer.inputProcessor(4);
                        emptyLifes.close();
                        clipRect.visible = false
                        clipRect.state = "2"
                    }
                }
            }


        }
    }

    Dialog {
        id: myDialog
        anchors.centerIn: parent
        width: 320
        height: 240
        closePolicy: Popup.CloseOnEscape
        background: Rectangle {
            //anchors.fill: parent
            radius: 30
            color: "#121318"
        }

        ColumnLayout {
            anchors.fill: parent
            spacing: 15

            Text {
                id: title
                anchors.topMargin: 2
                text: "Выберите игрока"
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
                font.bold: true
                font.family: "Century Gothic"
                font.pointSize: 24
                color: "white"
            }

            RowLayout {
                id: rowL
                Layout.alignment: Qt.AlignCenter
                spacing: 10

                Rectangle {
                    id: player1
                    Layout.alignment: Qt.AlignCenter
                    radius: 30
                    width: myDialog.width/2 - 35
                    height: rowL.height
                    visible: false
                    color: "#dbe1ff"

                    Text {
                        anchors.fill: parent
                        anchors.margins: 2
                        text: qsTr("Player 1")
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        font.family: "Century Gothic"
                        font.bold: true
                        font.pixelSize: 20
                        color: "#01174b"
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            connectionManager.connectToPlayer("e0:5a:1b:d1:6d:41");
                            myDialog.close();
                        }
                    }
                }

                Rectangle {
                    id: player2
                    Layout.alignment: Qt.AlignCenter
                    radius: 30
                    width: myDialog.width/2 - 35
                    height: rowL.height
                    visible: false
                    color: "#dbe1ff"

                    Text {
                        anchors.fill: parent
                        anchors.margins: 2
                        text: qsTr("Player 2")
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        font.family: "Century Gothic"
                        font.bold: true
                        font.pixelSize: 20
                        color: "#01174b"
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            connectionManager.connectToPlayer("08:3a:f2:aa:6d:09");
                            myDialog.close();
                        }
                    }

                }
            }

            RowLayout {
                id: rowl1
                Layout.alignment: Qt.AlignHCenter | Qt.AlignBottom
                anchors.bottom: parent.bottom
                spacing: 5

                Rectangle {
                    id: closeDialog
                    width: 100
                    height: 30
                    color: "transparent"

                    Text {
                        anchors.fill: parent
                        text: qsTr("Выйти")
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        font.family: "Century Gothic"
                        font.bold: true
                        font.pixelSize: 17
                        color: "white"
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            Qt.quit();
                        }
                    }
                }

                Rectangle {
                    id: retryScan
                    width: 100
                    height: 30
                    color: "transparent"

                    Text {
                        anchors.fill: parent
                        text: qsTr("Обновить")
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        font.family: "Century Gothic"
                        font.bold: true
                        font.pixelSize: 17
                        color: "white"
                    }
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            connectionManager.checkWiFiConnection();
                            myDialog.close();
                        }
                    }
                }
            }
        }


    }

    Dialog {
        id: permissionDialog
        anchors.centerIn: parent
        width: 320
        height: 240
        closePolicy: Popup.CloseOnEscape

        background: Rectangle {
            radius: 30
            color: "#121318"
        }

        ColumnLayout {
            id: permissionLayout
            anchors.fill: parent

            Text {
                id: titlePermission
                anchors.topMargin: 3
                text: "Ошибка разрешения"
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
                font.bold: true
                font.family: "Century Gothic"
                font.pointSize: 24
                color: "white"
            }

            Text {
                id: textPermission
                anchors.topMargin: 2
                width: 120
                text: "Предоставьте разрешение\nдля дальнейшей работы\nприложения!"
                wrapMode: Text.WordWrap
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                Layout.alignment: Qt.AlignHCenter
                font.bold: true
                font.family: "Century Gothic"
                font.pointSize: 18
                color: "white"
            }

            Rectangle {
                id: buttonPermission
                width: 150
                height: 40
                radius: 20
                color: "#dbe1ff"
                anchors.bottomMargin: 3
                Layout.alignment: Qt.AlignHCenter | Qt.AlignBottom

                Text {
                    anchors.fill: parent
                    anchors.margins: 1
                    text: qsTr("Предоставить")
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.family: "Century Gothic"
                    font.bold: true
                    font.pixelSize: 18
                    color: "#01174b"
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        connectionManager.openSettingsApp();
                        permissionDialog.close();
                        Qt.quit();
                    }
                }
            }
        }
    }

    Dialog {
            id: noAvaliablePlayersDialog
            anchors.centerIn: parent
            width: 540
            height: 180
            closePolicy: Popup.CloseOnEscape

            background: Rectangle {
                radius: 30
                color: "#121318"
            }

            ColumnLayout {
                id: noAvaliablePlayersLayout
                anchors.fill: parent

                Text {
                    id: titleNoAvaliablePlayers
                    anchors.topMargin: 3
                    text: "Ошибка"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
                    font.bold: true
                    font.family: "Century Gothic"
                    font.pointSize: 24
                    color: "white"
                }

                Text {
                    id: textNoAvaliablePlayers
                    anchors.topMargin: 2
                    width: 350
                    text: "Ни одна из моделей игроков недоступxна. Убедитесь, что\nигровая платформа подключена к питанию и включена!"
                    wrapMode: Text.WordWrap
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                    Layout.alignment: Qt.AlignHCenter
                    font.bold: true
                    font.family: "Century Gothic"
                    font.pointSize: 18
                    color: "white"
                }

                RowLayout {
                    spacing: 15
                    anchors.bottomMargin: 1
                    Layout.alignment: Qt.AlignBottom
                    Layout.preferredWidth: parent.width/2

                    Rectangle {
                        id: buttonQuitNoAvaliablePlayers
                        Layout.preferredWidth: parent.width/2
                        height: 40
                        radius: 20
                        color: "transparent"
                        Layout.alignment: Qt.AlignHCenter | Qt.AlignBottom

                        Text {
                            anchors.fill: parent
                            text: qsTr("Выйти")
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignHCenter
                            font.family: "Century Gothic"
                            font.bold: true
                            font.pixelSize: 18
                            color: "white"
                        }

                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                Qt.quit();
                            }
                        }
                    }

                    Rectangle {
                        id: buttonRetryNoAvaliablePlayers
                        Layout.fillWidth: true
                        height: 40
                        radius: 20
                        color: "transparent"
                        Layout.alignment: Qt.AlignHCenter | Qt.AlignBottom

                        Text {
                            anchors.fill: parent
                            text: qsTr("Попробовать снова")
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignHCenter
                            font.family: "Century Gothic"
                            font.bold: true
                            font.pixelSize: 18
                            color: "white"
                        }

                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                connectionManager.checkWiFiConnection();
                                noAvaliablePlayersDialog.close();
                            }
                        }
                    }
                }

            }
        }

    Item {
        id: clipRect
        anchors.centerIn: parent
        width: parent.width
        height: parent.height
        clip: true
        visible: true
        state: "1"
        opacity: 1

        GaussianBlur {
            x: rootRec.x
            y: rootRec.y
            width: rootRec.width
            height: rootRec.height
            source: rootRec
            radius: 32
            samples: 16
        }

        Rectangle {
            anchors.fill: parent
            color: "black"
            opacity: 0.7
        }

        states: [
            State {
                name: "2"
                PropertyChanges { target: clipRect; opacity: 0.0  }
            }
        ]

        transitions: [
            Transition {
                reversible: true
                PropertyAnimation { easing.type: Easing.InOutQuad; properties: "opacity"; duration: 400 }

            }
        ]
    }

    Item {
        id: sharinganComponent
        x: parent.width/2-10
        y: parent.height/2-10
        visible: true
        opacity: 1
        state: "1"
        SharinganLoader { radius: 20 }

        states: [
            State {
                name: "2"
                PropertyChanges { target: sharinganComponent; opacity: 0.0  }
            }
        ]

        transitions: [
            Transition {
                reversible: true
                PropertyAnimation { easing.type: Easing.InOutQuad; properties: "opacity"; duration: 400 }

            }
        ]
    }

    function openListPlayers(listPlayers) {
        //player1.visible = true
        //player2.visible = true

        if(listPlayers.length === 2) {
            player1.visible = true
            player2.visible = true
        } else {
            if(listPlayers[0] === "e0:5a:1b:d1:6d:41") {
                player1.visible = true
                player1.anchors.rightMargin = 0
            } else {
                player2.visible = true
                player2.anchors.leftMargin = 0
            }

        }

        myDialog.open()
    }

    function endConnection() {
        sharinganComponent.state = "2"
        clipRect.state = "2"
        clipRect.visible = false
        sharinganComponent.visible = false

    }

    function showPermissionDialog() {
        permissionDialog.open();
    }

    function showEmptyLifes() {
        clipRect.visible = true
        clipRect.state = '1'
        emptyLifes.open()
    }

    function showNoAvailable() {
        noAvaliablePlayersDialog.open();
    }
}
