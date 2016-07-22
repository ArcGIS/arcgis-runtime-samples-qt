// [WriteFile Name=TiledLayerRenderMode, Category=Layers]
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

import QtQuick 2.6
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import Esri.ArcGISExtras 1.1
import Esri.Samples 1.0


TiledLayerRenderModeSample {
    id: tiledLayerRenderMode
    clip: true
    width: 800
    height: 600

    property real scaleFactor: System.displayScaleFactor

    // add a mapView component
    MapView {
        anchors.fill: parent
        objectName: "mapView"
    }

    // switch to change RenderModes
    Switch {
        id: modeSwitch
        anchors {
            bottom: parent.bottom
            horizontalCenter: parent.horizontalCenter
            margins: 10 * scaleFactor
        }
        width: 140 * scaleFactor
        height: 25 * scaleFactor

        style: SwitchStyle {

            groove: Rectangle {
                width: modeSwitch.width
                height: modeSwitch.height
                radius: 3
                border {
                    color: "#4D4D4D"
                    width: 1
                }
                color: "#E0E0E0"
            }

            handle: Rectangle {
                width: modeSwitch.width / 2
                height: modeSwitch.height
                radius: 3
                border.color: "#4D4D4D"

                Text {
                    anchors {
                        verticalCenter: parent.verticalCenter
                        horizontalCenter: parent.horizontalCenter
                    }
                    text: !modeSwitch.checked ? "Aesthetics" : "Scale"
                    renderType: Text.NativeRendering
                    font.pixelSize: 11 * scaleFactor
                }
            }
        }

        onCheckedChanged: {
            tiledLayerRenderMode.changeRenderMode();
        }
    }

    // neatline rectangle
    Rectangle {
        anchors.fill: parent
        border {
            color: "black"
            width: 0.5 * scaleFactor
        }
        color: "transparent"
    }
}
