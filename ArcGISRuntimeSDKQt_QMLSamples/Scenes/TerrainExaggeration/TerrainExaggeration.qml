// [WriteFile Name=TerrainExaggeration, Category=Scenes]
// [Legal]
// Copyright 2018 Esri.

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
import Esri.ArcGISRuntime 100.3
import Esri.ArcGISExtras 1.1

Rectangle {
    id: rootRectangle
    clip: true
    width: 800
    height: 600

    property real scaleFactor: System.displayScaleFactor

    SceneView {
        id: sceneView
        anchors.fill: parent

        Scene {
            id: scene
            BasemapImagery {}
            Surface {
                id: surface
                ArcGISTiledElevationSource {
                    url: "http://elevation3d.arcgis.com/arcgis/rest/services/WorldElevation3D/Terrain3D/ImageServer"
                }
            }
            ViewpointCenter {
                Point {
                    id: initialPoint
                    x: 83.9
                    y: 28.4
                    z: 10010
                    spatialReference: SpatialReference { wkid: 4326 }
                }
                targetScale: 10010

                Camera {
                    location: initialPoint
                    heading: 41.64729875588979
                    pitch: 71.2017391571523
                    roll: 2.194677223e-314
                }
            }
        }
    }

    // Slider UI presentation at bottom
    Rectangle {
        height: 46 * scaleFactor
        anchors {
            horizontalCenter: parent.horizontalCenter
            bottom: parent.bottom
            bottomMargin: 20 * scaleFactor
        }

        // sliderCombo: A slider and text for its value
        Row {
            id: sliderCombo
            anchors {
                centerIn: parent
            }
            spacing: 5

            Slider {
                id: slider1
                anchors {
                    verticalCenter: parent.verticalCenter
                }
                style: SliderStyle {
                    groove: Rectangle {
                        implicitWidth: 200 * scaleFactor
                        implicitHeight: 8 * scaleFactor
                        color: "gray"
                        opacity: 0.7
                        radius: 8 * scaleFactor
                    }
                    handle: Rectangle {
                        anchors.centerIn: parent
                        color: control.pressed ? "white" : "lightgray"
                        border.color: "gray"
                        border.width: 2 * scaleFactor
                        implicitWidth: 34 * scaleFactor
                        implicitHeight: 34 * scaleFactor
                        radius: 12 * scaleFactor
                    }
                }
                // Slider controls the magnitude of exaggeration
                minimumValue: 1.0
                maximumValue: 10
                onValueChanged: {
                   surface.elevationExaggeration = Qt.binding(function() { return slider1.value })
                }
            }

            TextField {
                anchors {
                    verticalCenter: parent.verticalCenter
                }
                horizontalAlignment: TextInput.AlignHCenter
                style: TextFieldStyle {
                    textColor: "white"
                    background: Rectangle {
                        implicitWidth: 55 * scaleFactor
                        implicitHeight: 34 * scaleFactor
                        border.width: 0
                        radius: 5 * scaleFactor
                        color: "gray"
                        opacity: 0.7
                    }
                }
                readOnly: true
                font.pixelSize: 20 * scaleFactor
                text: slider1.value.toFixed(0)
            }
        }
    }
}
