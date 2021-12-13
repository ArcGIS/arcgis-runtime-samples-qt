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
import QtQuick.Layouts 1.12
import Esri.Samples 1.0
import QtQuick.Dialogs 1.2

Item {

    // add a mapView component
    MapView {
        id: view
        anchors.fill: parent

        Component.onCompleted: {
            // Set and keep the focus on SceneView to enable keyboard navigation
            forceActiveFocus();
        }

        Control {
            id: toggleControl
            anchors {
                top: view.top
                topMargin: 10
                left: view.left
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
                    text: model.dimensionLayerName
                }
                CheckBox {
                    id: visibilityToggle
                    text: "Dimension Layer visibility"
                    checked: true
                    onCheckStateChanged: model.dimensionLayerVisibility = visibilityToggle.checkState;
                    enabled: model.checkBoxesEnabled;
                }
                CheckBox {
                    id: definitionExpressionToggle
                    text: "Definition Expression: \nDimensions >= 450m"
                    checked: false
                    onCheckStateChanged: model.definitionExpressionApplied = definitionExpressionToggle.checkState;
                    enabled: model.checkBoxesEnabled && visibilityToggle.checked ? true : false;
                }
            }
        }

        // Pop-up error message box
        MessageDialog {
            id: errorMessageBox
            title: "Error message!"
            text: model.errorMessage
            icon: StandardIcon.Warning
            visible: model.errorMessage === "" ? false : true
            onAccepted: model.errorMessage = "";
        }
    }

    // Declare the C++ instance which creates the scene etc. and supply the view
    DisplayDimensionsSample {
        id: model
        mapView: view
    }
}
