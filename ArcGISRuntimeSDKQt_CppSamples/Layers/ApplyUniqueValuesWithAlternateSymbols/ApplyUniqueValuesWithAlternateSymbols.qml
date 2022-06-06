// [WriteFile Name=ApplyUniqueValuesWithAlternateSymbols, Category=Layers]
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
import QtQuick.Controls 2.12
import Esri.Samples 1.0

Item {

    // add a mapView component
    MapView {
        id: view
        anchors.fill: parent

        Component.onCompleted: {
            // Set and keep the focus on SceneView to enable keyboard navigation
            forceActiveFocus();
        }
    }

    Column {
        id: scaleColumn
        anchors {
            left: parent.left
            top: parent.top
            margins: 15
        }
        spacing: 5

        Button {
            width: 200
            enabled: true
            text: "Set Scale to 1:2500"
            onClicked:
            {
                model.setScale(2500);
            }
        }

        Button {
            width: 200
            enabled: true
            text: "Set Scale to 1:7500"
            onClicked: model.setScale(7500);
        }

        Button {
            width: 200
            enabled: true
            text: "Set Scale to 1:15000"
            onClicked: model.setScale(15000);
        }
    }

    // Declare the C++ instance which creates the scene etc. and supply the view
    ApplyUniqueValuesWithAlternateSymbolsSample {
        id: model
        mapView: view
    }
}
