import QtQuick 2.0

import "qrc:/qml"
import "qrc:/qml/js/constants.js" as Const

Rectangle {
    id: fileListHeaderRoot

    width: 480
    height: 62

    property alias nameVisible : nameHeader.visible
    property alias extVisible  : extHeader.visible
    property alias typeVisible : typeHeader.visible
    property alias sizeVisible : sizeHeader.visible
    property alias dateVisible : dateHeader.visible
    property alias ownerVisible: ownerHeader.visible
    property alias permsVisible: permsHeader.visible
    property alias attrVisible : attribsHeader.visible


    property alias nameWidth : nameHeader.width
    property alias extWidth  : extHeader.width
    property alias typeWidth : typeHeader.width
    property alias sizeWidth : sizeHeader.width
    property alias dateWidth : dateHeader.width
    property alias ownerWidth: ownerHeader.width
    property alias permsWidth: permsHeader.width
    property alias attrWidth : attribsHeader.width

    signal rightClicked(var posX, var posY);

    // Set Name Header Separator Position
    function setNameHeaderSeparatorPosition(xPos) {
        // Set Separator Position
        nameHeader.setSeparatorPosition(xPos);
    }

    color: Const.DEFAULT_FILE_LIST_HEADER_BACKGROUND_COLOR

    // Header Row
    Row {
        id: itemsContainer
        anchors.fill: parent
        //color: "transparent"

        // Header Item - Name
        FileListHeaderItem {
            id: nameHeader
            width: 152
            height: parent.height
            itemText: "Filename"
        }

        // Header Item - Ext
        FileListHeaderItem {
            id: extHeader
            height: parent.height
            itemText: "Ext"
            width: 40
            visible: mainController.extVisible
        }

        // Header Item - Type
        FileListHeaderItem {
            id: typeHeader
            width: 120
            height: parent.height
            itemText: "Type"
            visible: mainController.typeVisible
        }

        FileListHeaderItem {
            id: sizeHeader
            width: 60
            height: parent.height
            itemText: "Size"
            visible: mainController.sizeVisible
        }

        FileListHeaderItem {
            id: dateHeader
            width: 128
            height: parent.height
            itemText: "Date"
            visible: mainController.dateVisible
        }

        FileListHeaderItem {
            id: ownerHeader
            width: 60
            height: parent.height
            itemText: "Owner"
            visible: mainController.ownerVisible
        }

        FileListHeaderItem {
            id: permsHeader
            width: 100
            height: parent.height
            itemText: "Perms"
            visible: mainController.permsVisible
        }

        FileListHeaderItem {
            id: attribsHeader
            width: 60
            height: parent.height
            itemText: "Attrs"
            visible: mainController.attrsVisible
        }

    }

    MouseArea {
        anchors.fill: parent
        acceptedButtons: Qt.RightButton
        preventStealing: true
        // On Clicked
        onClicked: {
            // Emit Right Clicked
            rightClicked(mouse.x, mouse.y);
        }
    }

    // Connections - Main Controller
    Connections {
        target: mainController
    }
}

