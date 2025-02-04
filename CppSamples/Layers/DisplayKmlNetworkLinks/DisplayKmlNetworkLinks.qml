// [WriteFile Name=DisplayKmlNetworkLinks, Category=Layers]
// [Legal]
// Copyright 2018 Esri.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
// [Legal]

import QtQuick
import QtQuick.Controls
import QtQuick.Window
import Esri.Samples

DisplayKmlNetworkLinksSample {
    id: rootRectangle
    clip: true
    width: 800
    height: 600



    Dialog {
        id: messageDialog
        modal: true
        x: Math.round(parent.width - width) / 2
        y: Math.round(parent.height - height) / 2
        width: parent.width * 0.75
        standardButtons: Dialog.Ok
        title: "KML layer message"
        property alias text : textLabel.text
        Text {
            width: parent.width
            id: textLabel
            text: currentKmlNetworkMessage
            wrapMode: Text.WordWrap
        }
        onAccepted: {
            currentKmlNetworkMessage = ""; // Clear the message
        }
        onRejected: onAccepted()
    }

    SceneView {
        id: sceneView
        objectName: "sceneView"
        anchors.fill: parent

        Component.onCompleted: {
            // Set the focus on SceneView to initially enable keyboard navigation
            forceActiveFocus();
        }

        MessageButton {
            visible: currentKmlNetworkMessage.length > 0
            anchors {
                bottom: sceneView.attributionTop
                horizontalCenter: parent.horizontalCenter
                margins: 10
            }
            onClicked: {
                messageDialog.open();
            }
        }
    }
}
