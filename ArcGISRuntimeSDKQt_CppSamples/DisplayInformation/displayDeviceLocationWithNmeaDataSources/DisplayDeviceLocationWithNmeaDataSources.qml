// [WriteFile Name=DisplayDeviceLocationWithNmeaDataSources, Category=DisplayInformation]
// [Legal]
// Copyright 2020 Esri.

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
import QtQuick.Controls 2.2
import Esri.Samples 1.0

Item {

    // add a mapView component
    MapView {
        id: view
        anchors.fill: parent
    }

    // Declare the C++ instance which creates the scene etc. and supply the view
    DisplayDeviceLocationWithNmeaDataSourcesSample {
        id: model
        mapView: view
    }

    Button {
        id: button
        anchors.horizontalCenter: parent.horizontalCenter
        y: 5
        width: 200
        text: model.sampleStarted ? "Stop tracking" : "Start tracking"
        onClicked: {
            model.changeSampleState();
        }
    }

    Rectangle {
        height: dataSourceCol.height
        width: dataSourceCol.width
        x: parent.width - (width + 15)
        y: 5
        color: "white"
        opacity: .9
        border.color: "black"
        border.width: 1
        Column {
            id: dataSourceCol
            padding: 10
            Text {
                text: "Data source"
                font.bold: true

            }
            RadioButton {
                id: simData
                text: "Simulated data"
                checked: true
                onCheckedChanged: {
                    if (simData.checked)
                        model.changeDataSource();
                }
            }
            RadioButton {
                id: receiverData
                text: "Receiver"
                onCheckedChanged: {
                    if (receiverData.checked)
                        model.changeDataSource();
                }
            }
        }
    }
}
