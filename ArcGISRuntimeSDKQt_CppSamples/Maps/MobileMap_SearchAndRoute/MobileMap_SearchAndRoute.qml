// [WriteFile Name=MobileMap_SearchAndRoute, Category=Maps]
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
import Esri.Samples 1.0
import Esri.ArcGISExtras 1.1
import Esri.ArcGISRuntime.Toolkit.Controls 100.4

MobileMap_SearchAndRouteSample {
    id: mobileMapSearchRoute
    clip: true
    width: 800
    height: 600

    property double scaleFactor: System.displayScaleFactor
    property string dataPath: System.userHomeFolder.path + "/ArcGIS/Runtime/Data/mmpk"
    property bool isMapOpen: false

    // make background a light gray
    Rectangle {
        anchors.fill: parent
        color: "#E0E0E0"

        // add a mapView component
        MapView {
            id: mapView
            objectName: "mapView"

            // callout to display information
            Callout {
                calloutData: mobileMapSearchRoute.calloutData
                screenOffsetY: -19 * scaleFactor
                accessoryButtonHidden: true
            }

            Rectangle {
                anchors {
                    left: parent.left
                    top: parent.top
                }
                opacity: 0.50
                height: parent.height
                width: 25 * scaleFactor
                color: "#E0E0E0"

                Rectangle {
                    anchors {
                        left: parent.left
                        top: parent.top
                    }
                    width: parent.width
                    height: 100 * scaleFactor
                    color: "#283593"

                    Image {
                        anchors.centerIn: parent
                        mirror: true
                        source: "qrc:/Samples/Maps/MobileMap_SearchAndRoute/forwardIcon.png"
                        height: 33 * scaleFactor
                        width: height
                    }
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        mapSelectionStack.pop();
                    }
                }
            }

            // Map controls
            Column {
                anchors {
                    top: parent.top
                    right: parent.right
                    margins: 10 * scaleFactor
                }
                spacing: 10 * scaleFactor

                // solve route button
                Rectangle {
                    id: routeButton
                    color: "#E0E0E0"
                    height: 50 * scaleFactor
                    width: height
                    border.color: "black"
                    radius: 2
                    opacity: 0.90
                    visible: mobileMapSearchRoute.canRoute

                    Image {
                        anchors {
                            centerIn: parent
                            margins: 5 * scaleFactor
                        }
                        source: "qrc:/Samples/Maps/MobileMap_SearchAndRoute/routingSymbol.png"
                        height: 44 * scaleFactor
                        width: height
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            mobileMapSearchRoute.solveRoute();
                        }
                    }
                }

                // clear graphics button
                Rectangle {
                    id: clearButton
                    color: "#E0E0E0"
                    height: 50 * scaleFactor
                    width: height
                    border.color: "black"
                    radius: 2
                    opacity: 0.90
                    visible: mobileMapSearchRoute.canClear

                    Image {
                        anchors {
                            centerIn: parent
                            margins: 5 * scaleFactor
                        }
                        source: "qrc:/Samples/Maps/MobileMap_SearchAndRoute/discardSymbol.png"
                        height: 44 * scaleFactor
                        width: height
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            mobileMapSearchRoute.resetMapView();
                        }
                    }
                }
            }
        }

        // use a StackView for the user interface
        StackView {
            id: mapSelectionStack
            anchors.fill: parent

            // begin with the mobile map package selection options
            initialItem: Item {

                Column {
                    anchors {
                        top: parent.top
                        left: parent.left
                    }
                    width: parent.width
                    spacing: 20 * scaleFactor

                    // UI navigation bar
                    Rectangle {
                        width: parent.width
                        height: 100 * scaleFactor
                        color: "#283593"

                        // forward navigation button. Visible after first map is selected
                        Image {
                            anchors {
                                verticalCenter: parent.verticalCenter
                                right: parent.right
                                margins: 10 * scaleFactor
                            }
                            source: "qrc:/Samples/Maps/MobileMap_SearchAndRoute/forwardIcon.png"
                            height: 44 * scaleFactor
                            width: height
                            visible: isMapOpen

                            MouseArea {
                                anchors.fill: parent
                                onClicked: {
                                    mapSelectionStack.push(mapSelectView);
                                }
                            }
                        }

                        Text {
                            anchors.centerIn: parent
                            color: "white"
                            height: 40 * scaleFactor
                            font.pixelSize: 25 * scaleFactor
                            text: "Choose a Mobile Map Package"
                            renderType: Text.NativeRendering
                        }
                    }

                    // mobile map package ListView
                    ListView {
                        anchors.horizontalCenter: parent.horizontalCenter
                        height: 400 * scaleFactor
                        width: 200 * scaleFactor
                        spacing: 10 * scaleFactor
                        model: mobileMapSearchRoute.mmpkList

                        delegate: Component {
                            Rectangle {

                                width: 200 * scaleFactor
                                height: 50 * scaleFactor
                                color: "#283593"
                                radius: 2
                                border.color: "darkgray"

                                Text {
                                    anchors.centerIn: parent
                                    horizontalAlignment: Text.AlignHCenter
                                    color: "white"
                                    width: 150 * scaleFactor
                                    text: modelData
                                    renderType: Text.NativeRendering
                                    elide: Text.ElideMiddle
                                }

                                MouseArea {
                                    anchors.fill: parent
                                    propagateComposedEvents: false
                                    onClicked: {
                                        isMapOpen = false;

                                        //generate list of maps and go to map selection view
                                        mobileMapSearchRoute.createMapList(index);
                                        mapSelectionStack.push(mapSelectView);
                                    }
                                }
                            }
                        }
                    }
                }
            }

            Item {
                id: mapSelectView

                Column {
                    anchors {
                        top: parent.top
                        left: parent.left
                    }
                    width: parent.width
                    spacing: 20 * scaleFactor

                    // UI navigation bar
                    Rectangle {
                        width: parent.width
                        height: 100 * scaleFactor
                        color: "#283593"

                        // back button
                        Image {
                            anchors {
                                verticalCenter: parent.verticalCenter
                                left: parent.left
                                margins: 10 * scaleFactor
                            }
                            mirror: true
                            source: "qrc:/Samples/Maps/MobileMap_SearchAndRoute/forwardIcon.png"
                            height: 44 * scaleFactor
                            width: height

                            MouseArea {
                                anchors.fill: parent
                                onClicked: {
                                    mapSelectionStack.pop();
                                }
                            }
                        }

                        // forward button. Only visible after first map has been selected
                        Image {
                            anchors {
                                verticalCenter: parent.verticalCenter
                                right: parent.right
                                margins: 10 * scaleFactor
                            }
                            source: "qrc:/Samples/Maps/MobileMap_SearchAndRoute/forwardIcon.png"
                            height: 44 * scaleFactor
                            width: height
                            visible: isMapOpen

                            MouseArea {
                                anchors.fill: parent
                                onClicked: {
                                    mapSelectionStack.push(mapView);
                                }
                            }
                        }

                        Text {
                            anchors.centerIn: parent
                            color: "white"
                            height: 40 * scaleFactor
                            font.pixelSize: 25 * scaleFactor
                            text: "Choose a Map"
                            renderType: Text.NativeRendering
                        }
                    }

                    // list of maps
                    ListView {
                        anchors.horizontalCenter: parent.horizontalCenter
                        height: 400 * scaleFactor
                        width: 200 * scaleFactor
                        spacing: 10 * scaleFactor
                        model: mobileMapSearchRoute.mapList

                        delegate: Component {
                            Rectangle {
                                width: 200 * scaleFactor
                                height: 50 * scaleFactor
                                color: "#283593"
                                radius: 2
                                border.color: "darkgray"

                                Text {
                                    anchors.centerIn: parent
                                    horizontalAlignment: Text.AlignHCenter
                                    color: "white"
                                    width: 150 * scaleFactor
                                    text: modelData.name
                                    renderType: Text.NativeRendering
                                    elide: Text.ElideMiddle
                                }

                                // geocoding available icon
                                Image {
                                    anchors {
                                        left: parent.left
                                        top: parent.top
                                        margins: 5 * scaleFactor
                                    }
                                    source: "qrc:/Samples/Maps/MobileMap_SearchAndRoute/pinOutlineSymbol.png"
                                    height: 20 * scaleFactor
                                    width: height
                                    visible: modelData.geocoding
                                }

                                // routing available icon
                                Image {
                                    anchors {
                                        right: parent.right
                                        top: parent.top
                                        margins: 5 * scaleFactor
                                    }
                                    source: "qrc:/Samples/Maps/MobileMap_SearchAndRoute/routingSymbol.png"
                                    height: 20 * scaleFactor
                                    width: height
                                    visible: modelData.routing
                                }

                                MouseArea {
                                    anchors.fill: parent
                                    propagateComposedEvents: false
                                    onClicked: {
                                        // set map and display mapView
                                        mobileMapSearchRoute.selectMap(index);
                                        isMapOpen = true;
                                        mapSelectionStack.push(mapView);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        BusyIndicator {
            anchors.centerIn: parent
            visible: mobileMapSearchRoute.isGeocodeInProgress
        }
    }
}
