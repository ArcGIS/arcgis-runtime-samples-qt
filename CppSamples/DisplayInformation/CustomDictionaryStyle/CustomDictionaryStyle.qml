// [WriteFile Name=CustomDictionaryStyle, Category=DisplayInformation]
// [Legal]
// Copyright 2019 Esri.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
// [Legal]

import QtQuick
import QtQuick.Controls
import Esri.Samples

Item {

    // add a mapView component
    MapView {
        id: view
        anchors.fill: parent

        Component.onCompleted: {
            // Set the focus on MapView to initially enable keyboard navigation
            forceActiveFocus();
        }
    }

    // Declare the C++ instance which creates the map etc. and supply the view
    CustomDictionaryStyleSample {
        id: model
        mapView: view
    }

    Rectangle {
        id: rectangle
        anchors {
            left: parent.left
            top: parent.top
            margins: 5
        }
        width: radioColumn.width
        height: radioColumn.height
        color: "white"
        border {
            color: "black"
            width: 1
        }
        opacity: 0.9

        Column {
            id: radioColumn
            padding: 5
            spacing: 5

            Text {
                text: "Custom Dictionary Symbol Style Source"
            }

            RadioButton {
                text: "Local .stylx file"
                checked: true
            }

            RadioButton {
                text: "Web style"
                onCheckedChanged: {
                    model.changeDictionarySymbolStyleSource();
                }
            }
        }
    }
}
