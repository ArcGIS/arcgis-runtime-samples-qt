// [WriteFile Name=OpenMobileMap_MapPackage, Category=Maps]
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
import Esri.ArcGISRuntime 100.0
import Esri.ArcGISExtras 1.1

Rectangle {
    width: 800
    height: 600

    property real scaleFactor: System.displayScaleFactor
    property url dataPath: System.userHomePath + "/ArcGIS/Runtime/Data/mmpk/"

    // Create MapView
    MapView {
        id: mapView
        anchors.fill: parent
    }

    // Create a Mobile Map Package and set the path
    MobileMapPackage {
        id: mmpk
        path: dataPath + "Yellowstone.mmpk"

        // load the mobile map package
        Component.onCompleted: {
            mmpk.load();
        }

        // wait for the mobile map package to load
        onLoadStatusChanged: {
            if (loadStatus === Enums.LoadStatusLoaded) {
                // set the map view's map to the first map in the mobile map package
                mapView.map = mmpk.maps[0];
            }
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
