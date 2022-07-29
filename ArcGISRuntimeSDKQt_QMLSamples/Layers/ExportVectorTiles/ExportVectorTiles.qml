// [WriteFile Name=ExportVectorTiles, Category=Layers]
// [Legal]
// Copyright 2022 Esri.

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

import QtQuick 2.12
import Esri.ArcGISRuntime 100.15
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.15
import Esri.ArcGISExtras 1.1

Rectangle {
    id: rootRectangle
    clip: true
    width: 800
    height: 600

    property int exportProgress: 0
    property int exportJobStatus: 0

    // add a mapView component
    MapView {
        id: mapView
        anchors.fill: parent

        Map {
            id: map
            initBasemapStyle: Enums.BasemapStyleArcGISStreetsNight
            initialViewpoint: ViewpointCenter {
                id: viewpoint
                Point {
                    x: -117.181
                    y: 34.049
                    spatialReference: SpatialReference { wkid: 4326 }
                }
                targetScale: 1e4
            }
        }

        GraphicsOverlay {
            id: graphicsOverlay

            Graphic {
                id: exportAreaGraphic
                symbol: SimpleLineSymbol {
                    style: Enums.SimpleLineSymbolStyleSolid
                    color: "green"
                    width: 3
                }
            }
        }

        Component.onCompleted: {
            // Set and keep the focus on MapView to enable keyboard navigation
            forceActiveFocus();
        }
    }

    Rectangle {
        id: exportProgressWindow
        anchors.centerIn: parent

        color: "white"
        visible: exportJobStatus !== 0 && exportJobStatus !== 3 && exportJobStatus !== 4

        border {
            color: "black"
            width: 2
        }

        Column {
            id: exportProgressColumn
            anchors.centerIn: parent
            spacing: 10

            BusyIndicator {
                id: busyIndicator
                anchors.horizontalCenter: parent.horizontalCenter
            }

            Text {
                id: statusText
                anchors.horizontalCenter: parent.horizontalCenter
                text: "Export job status: " + ["Not started", "Started", "Paused", "Succeeded", "Failed", "Cancelling"][exportJobStatus]
                font.pixelSize: 16
            }

            Text {
                id: progressText
                anchors.horizontalCenter: parent.horizontalCenter
                text: exportProgress + "% Completed"
                font.pixelSize: 16
            }

            onWidthChanged: {
                exportProgressWindow.width = exportProgressColumn.width + 20
            }

            onHeightChanged: {
                exportProgressWindow.height = exportProgressColumn.height + 20
            }
        }
    }

    // Create an extent rectangle for selecting the offline area
    Rectangle {
        id: extentRectangle
        anchors.centerIn: parent

        width: parent.width - parent.width * 0.1
        height: parent.height - parent.height * 0.25
        color: "transparent"
        border {
            color: "red"
            width: 3
        }

        visible: exportJobStatus === 0 || exportJobStatus === 4
    }

    // Button to start the download
    Button {
        id: button
        anchors {
            bottom: parent.bottom
            bottomMargin: rootRectangle.height * .05
            horizontalCenter: parent.horizontalCenter
        }
        width: 150

        text: "Export area"

        onClicked: {
            switch(exportJobStatus) {
            case 0: // Not started
                startExport(extentRectangle.x, (extentRectangle.y + extentRectangle.height), (extentRectangle.x + extentRectangle.width), extentRectangle.y);
                break;
            case 1: // Started
                cancel();
                break;
            case 2: // Paused
                break;
            case 3: // Succeeded
                reset();
                break;
            case 4: // Failed
                startExport(extentRectangle.x, (extentRectangle.y + extentRectangle.height), (extentRectangle.x + extentRectangle.width), extentRectangle.y);
                break;
            case 5: // Cancelling
                break;
            default:
                break;
            }
        }
    }

    function startExport(xSW, ySW, xNE, yNE) {
        const vectorTiledLayer = map.basemap.baseLayers.get(0);
        if (vectorTiledLayer.layerType !== Enums.LayerTypeArcGISVectorTiledLayer)
            return;

        // Create an envelope from the QML rectangle corners
        const pointSW = mapView.screenToLocation(xSW, ySW);
        const pointNE = mapView.screenToLocation(xNE, yNE);
        const extent = ArcGISRuntimeEnvironment.createObject("Envelope", {
            xMin: pointSW.x,
            xMax: pointNE.x,
            yMin: pointNE.y,
            yMax: pointSW.y
        });
        const exportArea = GeometryEngine.normalizeCentralMeridian(GeometryEngine.project(extent, vectorTiledLayer.spatialReference));
        extentRectangle.visible = false;
        exportAreaGraphic.geometry = exportArea;

        exportVectorTilesTask.url = vectorTiledLayer.url;

        // Instantiate export parameters to create the export job with
        exportVectorTilesTask.createDefaultExportVectorTilesParameters(exportArea, mapView.mapScale * 0.1);
    }

    ExportVectorTilesTask {
        id: exportVectorTilesTask

        property ExportVectorTilesJob exportVectorTilesJob;

        onCreateDefaultExportVectorTilesParametersStatusChanged: {
            if (createDefaultExportVectorTilesParametersStatus !== Enums.TaskStatusCompleted)
                return;

            // Using the reduced fonts service will reduce the download size of a vtpk by around 80 Mb
            // It is useful for taking the basemap offline but not recommended if you plan to later upload the vtpk
            defaultExportVectorTilesParameters.esriVectorTilesDownloadOption = Enums.EsriVectorTilesDownloadOptionUseReducedFontsService;

            // Create a path to store the vector tile package
            const vectorTileCachePath = System.temporaryFolder.url + "/vectorTiles_%1.vtpk".arg(new Date().getTime().toString());
            // Create a path to store the styling resources (in this case, the night mode version of the layer)
            const itemResourcePath = System.temporaryFolder.url + "/itemResources_%1".arg(new Date().getTime().toString());

            exportVectorTilesJob = exportVectorTilesWithStyleResources(defaultExportVectorTilesParameters, vectorTileCachePath, itemResourcePath);

            exportVectorTilesJob.resultChanged.connect(() => {
                if (exportVectorTilesJob.result) {
                    // Create a vector tiled layer when the download is completed
                    const exportedVectorTiledLayer = ArcGISRuntimeEnvironment.createObject("ArcGISVectorTiledLayer", {
                        vectorTileCache: exportVectorTilesJob.result.vectorTileCache,
                        itemResourceCache: exportVectorTilesJob.result.itemResourceCache
                    });
                    const basemap = ArcGISRuntimeEnvironment.createObject("Basemap");
                    basemap.baseLayers.append(exportedVectorTiledLayer);
                    map.basemap = basemap;
                    exportProgressWindow.visible = false;
                }
            });

            // Display the download progress to the user
            exportVectorTilesJob.progressChanged.connect(() => {
               exportProgress = exportVectorTilesJob.progress;
            });

            exportVectorTilesJob.statusChanged.connect((status) => {
                exportJobStatus = status;
                switch(exportJobStatus) {
                case 0: // Not started
                    button.text = "Export area"
                    break;
                case 1: // Started
                    button.text = "Cancel export"
                    break;
                case 2: // Paused
                    break;
                case 3: // Succeeded
                    button.text = "Reset"
                    break;
                case 4: // Failed
                    button.text = "Export area"
                    break;
                case 5: // Cancelling
                    break;
                default:
                    break;
                }
            });

            // Start the export job once export parameters have been created
            exportVectorTilesJob.start();
        }
    }

    function cancel() {
        exportVectorTilesTask.exportVectorTilesJob.cancelAsync();
        reset();
    }

    function reset() {
        map.basemap = ArcGISRuntimeEnvironment.createObject("Basemap", {initStyle: Enums.BasemapStyleArcGISStreetsNight});
        exportAreaGraphic.geometry = ArcGISRuntimeEnvironment.createObject("Geometry");
        extentRectangle.visible = true;
        button.text = "Export area"
        exportJobStatus = 0;
    }
}

