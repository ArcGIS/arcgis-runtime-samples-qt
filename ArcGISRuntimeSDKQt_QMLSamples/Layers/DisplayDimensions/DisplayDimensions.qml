// [WriteFile Name=DisplayDimensions, Category=Layers]
// [Legal]
// Copyright 2021 Esri.

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
import QtQuick.Controls 2.12
import Esri.ArcGISRuntime 100.13
import Esri.ArcGISExtras 1.1
import QtQuick.Layouts 1.12
import QtQuick.Dialogs 1.2

Rectangle {
    id: rootRectangle
    clip: true
    width: 800
    height: 600

    readonly property url dataPath: System.userHomePath + "/ArcGIS/Runtime/Data/mmpk/"
    property string errorMessage: ""

    property int indexOfDimensionLayer: 0

    MapView {
        id: mapView
        anchors.fill: parent

        Component.onCompleted: {
            // Set and keep the focus on MapView to enable keyboard navigation
            forceActiveFocus();
        }

        Control {
            id: toggleControl
            anchors {
                top: mapView.top
                topMargin: 10
                left: mapView.left
                leftMargin: 10
            }
            background: Rectangle {
                color: "white"
                border.color: "black"
            }
            padding: 5

            contentItem: GridLayout {
                columns: 1
                Text {
                    id: toggleBoxTitle
                }
                CheckBox {
                    id: visibilityToggle
                    text: "Dimension Layer visibility"
                    checked: true
                    onCheckStateChanged: mmpk.maps[0].operationalLayers.get(indexOfDimensionLayer).visible = visibilityToggle.checkState;
                    enabled: mmpk.loadStatus === Enums.LoadStatusLoaded ? true : false
                }
                CheckBox {
                    id: definitionExpressionToggle
                    text: "Definition Expression: \nDimensions >= 450m"
                    checked: false
                    onCheckStateChanged: {
                        if (definitionExpressionToggle.checkState == Qt.Checked) {
                            mmpk.maps[0].operationalLayers.get(indexOfDimensionLayer).definitionExpression = "DIMLENGTH >= 450";
                        }
                        else {
                            mmpk.maps[0].operationalLayers.get(indexOfDimensionLayer).definitionExpression = "";
                        }
                    }
                    enabled: mmpk.loadStatus === Enums.LoadStatusLoaded && visibilityToggle.checked ? true : false
                }
            }
        }

        // Pop-up error message box
        MessageDialog {
            id: errorMessageBox
            title: "Error message!"
            text: errorMessage
            icon: StandardIcon.Warning
            visible: errorMessage === "" ? false : true;
            onAccepted: errorMessage = "";
        }
    }

    Component.onCompleted: mmpk.load();

    MobileMapPackage {
        id: mmpk
        path: dataPath + "Edinburgh_Pylon_Dimensions.mmpk"

        onLoadStatusChanged: {
            if (loadStatus === Enums.LoadStatusFailedToLoad) {
                handleError(mmpk.loadError);
            }

            if (loadStatus !== Enums.LoadStatusLoaded) {
                return;
            }

            if (mmpk.maps.length < 1) {
                return;
            }

            // Find dimension layer and set toggleBoxTitle.
            for (let counter = 0; counter < mmpk.maps[0].operationalLayers.count; counter++) {
                if (mmpk.maps[0].operationalLayers.get(counter).layerType === Enums.LayerTypeDimensionLayer) {
                    indexOfDimensionLayer = counter;
                    toggleBoxTitle.text = mmpk.maps[0].operationalLayers.get(counter).name;
                }
            }

            // Set viewpoint.
            var viewpointPoint = ArcGISRuntimeEnvironment.createObject("Point", { x: -3.3098678, y: 55.9074044, spatialReference: Factory.SpatialReference.createWgs84()});
            var viewpointCenter = ArcGISRuntimeEnvironment.createObject("ViewpointCenter", {center: viewpointPoint, targetScale: 30000});
            mmpk.maps[0].initialViewpoint = viewpointCenter;

            // Set the map view's map to the first map in the mobile map package
            mapView.map = mmpk.maps[0];
            mapView.map.minScale = 35000;
        }
    }

    function handleError(error) {
        if (!error.additionalMessage)
            errorMessage = error.message;
        else
            errorMessage = error.message + "\n" + error.additionalMessage;
    }
}
