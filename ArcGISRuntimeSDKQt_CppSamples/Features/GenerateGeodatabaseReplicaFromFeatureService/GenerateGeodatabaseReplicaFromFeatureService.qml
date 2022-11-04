// [WriteFile Name=GenerateGeodatabaseReplicaFromFeatureService, Category=Features]
// [Legal]
// Copyright 2016 Esri.

// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0

// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
// [Legal]

import QtQuick
import QtQuick.Controls
import Esri.Samples

GenerateGeodatabaseReplicaFromFeatureServiceSample {
    id: generateSample
    width: 800
    height: 600

    property string statusText: ""

    // add a mapView component
    MapView {
        id: mapView
        anchors.fill: parent
        objectName: "mapView"

        Component.onCompleted: {
            // Set the focus on MapView to initially enable keyboard navigation
            forceActiveFocus();
        }
    }

    onHideWindow: (time, success) => {
        generateWindow.hideWindow(time);

        if (success) {
            extentRectangle.visible = false;
            downloadButton.visible = false;
        }
    }

    onUpdateStatus: status => statusText = status;

    Rectangle {
        id: extentRectangle
        anchors.centerIn: parent
        width: parent.width - 50
        height: parent.height - 125
        color: "transparent"
        border {
            color: "red"
            width: 3
        }
    }

    // Create the download button to generate geodatabase
    Rectangle {
        id: downloadButton
        property bool pressed: false
        anchors {
            horizontalCenter: parent.horizontalCenter
            bottom: parent.bottom
            bottomMargin: 23
        }

        width: 200
        height: 35
        color: pressed ? "#959595" : "#D6D6D6"
        radius: 8
        border {
            color: "#585858"
            width: 1
        }

        Row {
            anchors.fill: parent
            spacing: 5
            Image {
                width: 38
                height: width
                source: "qrc:/Samples/Features/GenerateGeodatabaseReplicaFromFeatureService/download.png"
            }

            Text {
                anchors.verticalCenter: parent.verticalCenter
                text: "Generate Geodatabase"
                font.pixelSize: 14
                color: "#474747"
            }
        }

        MouseArea {
            anchors.fill: parent
            onPressed: downloadButton.pressed = true
            onReleased: downloadButton.pressed = false
            onClicked: {
                generateSample.generateGeodatabaseFromCorners(extentRectangle.x, extentRectangle.y, (extentRectangle.x + extentRectangle.width), (extentRectangle.y + extentRectangle.height));
                generateWindow.visible = true;
            }
        }
    }

    // Create a window to display the generate window
    Rectangle {
        id: generateWindow
        anchors.fill: parent
        color: "transparent"
        visible: false
        clip: true

        Rectangle {
            anchors.fill: parent
            color: "#80808080"
        }

        MouseArea {
            anchors.fill: parent
            onClicked: mouse => mouse.accepted = true
            onWheel: wheel => wheel.accepted = true
        }

        Rectangle {
            anchors.centerIn: parent
            width: 125
            height: 100
            color: "lightgrey"
            opacity: 0.8
            radius: 5
            border {
                color: "#4D4D4D"
                width: 1
            }

            Column {
                anchors {
                    fill: parent
                    margins: 10
                }
                spacing: 10

                BusyIndicator {
                    anchors.horizontalCenter: parent.horizontalCenter
                }

                Text {
                    anchors.horizontalCenter: parent.horizontalCenter
                    text: statusText
                    font.pixelSize: 16
                }
            }
        }

        Timer {
            id: hideWindowTimer

            onTriggered: generateWindow.visible = false;
        }

        function hideWindow(time) {
            hideWindowTimer.interval = time;
            hideWindowTimer.restart();
        }
    }
}
