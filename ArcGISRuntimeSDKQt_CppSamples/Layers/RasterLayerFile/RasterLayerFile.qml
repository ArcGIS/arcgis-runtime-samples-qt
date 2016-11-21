// [WriteFile Name=RasterLayerFile, Category=Analysis]
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
import QtQuick.Dialogs 1.2
import Esri.Samples 1.0
import Esri.ArcGISExtras 1.1

RasterLayerFileSample {
    id: rootRectangle
    clip: true

    width: 800
    height: 600

    property double scaleFactor: System.displayScaleFactor
    property string dataPath: System.userHomePath + "/ArcGIS/Runtime/Data/raster/"

    // add a mapView component
    MapView {
        anchors.fill: parent
        objectName: "mapView"
    }

    Column {
        anchors {
            left: parent.left
            top: parent.top
            margins: 15
        }
        spacing: 10

        Button {
            text: "Add Raster"
            width: 100 * scaleFactor
            onClicked: fileDialog.open();
        }
    }

    FileDialog {
        id: fileDialog

        // only display supported raster formats
        nameFilters: ["Raster files (*.img *.I12 *.dt0 *.dt1 *.dt2 *.tc2 *.geotiff *.tif *.hr1 *.jpg *.jpeg *.jp2 *.ntf *.png *.i21 *.ovr)"]
        folder: dataPath

        onAccepted: {
            createAndAddRasterLayer(fileDialog.fileUrl)
        }
    }

    Rectangle {
        anchors.fill: parent
        color: "transparent"
        border {
            width: 0.5 * scaleFactor
            color: "black"
        }
    }
}
