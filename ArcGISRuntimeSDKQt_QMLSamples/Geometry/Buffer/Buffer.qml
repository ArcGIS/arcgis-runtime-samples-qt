// [WriteFile Name=Buffer, Category=Geometry]
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
import Esri.ArcGISRuntime 100.3
import Esri.ArcGISExtras 1.1

Rectangle {
    id: rootRectangle
    clip: true
    width: 800
    height: 600

    property real scaleFactor: System.displayScaleFactor

    // declare a map view
    MapView {
        id: mapView
        anchors.fill: parent

        // Add a map with a basemap
        Map {
            BasemapTopographic {}
        }

        // handle the mouse click - perform a buffer on click
        onMouseClicked: bufferPoint(mouse.mapPoint);

        // create graphics overlay for geodesic buffer graphics
        GraphicsOverlay {
            id: graphicsOverlaygeodesic

            // Set the renderer
            SimpleRenderer {
                // create a simple fill symbol for the geodesic buffer result
                SimpleFillSymbol {
                    id: geodesicBufferSymbol
                    color: Qt.rgba(1, 0, 0, 0.5)
                    style: Enums.SimpleFillSymbolStyleSolid

                    SimpleLineSymbol {
                        color: geodesicBufferSymbol.color
                        style: Enums.SimpleLineSymbolStyleSolid
                        width: 2
                    }
                }
            }
        }

        // create graphics overlay for planar buffer graphics
        GraphicsOverlay {
            id: graphicsOverlayPlanar

            // Set the renderer
            SimpleRenderer {
                // create a simple fill symbol for the planar buffer result
                SimpleFillSymbol {
                    id: planarBufferSymbol
                    color: Qt.rgba(0, 0, 1, 0.5)
                    style: Enums.SimpleFillSymbolStyleSolid

                    SimpleLineSymbol {
                        color: planarBufferSymbol.color
                        style: Enums.SimpleLineSymbolStyleSolid
                        width: 2
                    }
                }
            }
        }

        // create graphics overlay for the mouse clicks
        GraphicsOverlay {
            id: graphicsOverlayPoints

            // Set the renderer
            SimpleRenderer {
                // create a marker symbol for the clicked point
                SimpleMarkerSymbol {
                    id: pointSymbol
                    color: "white"
                    size: 14
                    style: Enums.SimpleMarkerSymbolStyleCross
                }
            }
        }
    }

    // function to buffer a point and add graphics
    function bufferPoint(point) {
        // Create a variable to be the buffer size in meters. There are 1609.34 meters in one mile.
        var bufferInMeters = bufferSizeText.text * 1609.34;

        // Create a planar buffer graphic around the input location at the specified distance.
        var buffer = GeometryEngine.buffer(point, bufferInMeters);

        // Add the result planar buffer as a graphic
        var resultGraphic = ArcGISRuntimeEnvironment.createObject("Graphic", {
                                                                      geometry: buffer
                                                                  });
        graphicsOverlayPlanar.graphics.append(resultGraphic);

        // Create a geodesic buffer graphic using the same location and distance.
        var bufferGeodesic = GeometryEngine.bufferGeodetic(point, bufferInMeters, Enums.LinearUnitIdMeters, NaN, Enums.geodesicCurveTypeGeodesic);

        // Add the result planar buffer as a graphic
        var resultGraphicGeodesic = ArcGISRuntimeEnvironment.createObject("Graphic", {
                                                                              geometry: bufferGeodesic
                                                                          });
        graphicsOverlaygeodesic.graphics.append(resultGraphicGeodesic);

        // Add the clicked point as a graphic
        var clickedPointGraphic = ArcGISRuntimeEnvironment.createObject("Graphic", {
                                                                            geometry: point
                                                                        });
        graphicsOverlayPoints.graphics.append(clickedPointGraphic);
    }

    // display a control pane to change size
    Rectangle {
        anchors {
            margins: -5 * scaleFactor
            fill: controlColumn
        }
        color: "lightgray"
        radius: 5 * scaleFactor
        border {
            color: "darkgray"
            width: 1 * scaleFactor
        }
    }

    Column {
        id: controlColumn
        anchors {
            left: parent.left
            top: parent.top
            margins: 10 * scaleFactor
        }
        spacing: 5 * scaleFactor

        Text {
            text: "Buffer size (miles):"
        }

        TextField {
            id: bufferSizeText
            validator: IntValidator { bottom: 1; top: 10000 }
            text: "1000"
        }

        Row {
            spacing: 10 * scaleFactor

            Rectangle {
                radius: 100 * scaleFactor
                width: 15 * scaleFactor
                height: width
                color: planarBufferSymbol.color
            }

            Text {
                text: "Planar Buffer"
            }
        }

        Row {
            spacing: 10 * scaleFactor

            Rectangle {
                radius: 100 * scaleFactor
                width: 15 * scaleFactor
                height: width
                color: geodesicBufferSymbol.color
            }

            Text {
                text: "Geodesic Buffer"
            }
        }

        Button {
            anchors.horizontalCenter: parent.horizontalCenter
            text: "Clear"
            onClicked: {
                graphicsOverlaygeodesic.graphics.clear();
                graphicsOverlayPlanar.graphics.clear();
                graphicsOverlayPoints.graphics.clear();
            }
        }
    }
}
