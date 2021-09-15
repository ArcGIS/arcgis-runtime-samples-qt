// [WriteFile Name=Query_OGC, Category=Layers]
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
import Esri.Samples 1.0
import QtQuick.Controls 1.4

Item {

    // add a mapView component
    MapView {
        id: view
        anchors.fill: parent
    }

    // Declare the C++ instance which creates the scene etc. and supply the view
    Query_OGCSample {
        id: model
        mapView: view
    }

    Rectangle {
        anchors {
            fill: controlColumn
            margins: -5
        }
        color: "#efefef"
        radius: 5
        border {
            color: "darkgray"
            width: 1
        }
    }

    Column {
        id: controlColumn
        anchors {
            left: parent.left
            top: parent.top
            margins: 10
        }
        spacing: 5

        Row {
            spacing: 5
            Text {
                id: clause_text
                anchors.verticalCenter: parent.verticalCenter
                text: "Where Clause"
            }

            ComboBox {
                id: clause_menu
                width: 200
                model: [
                    "F_CODE = 'AP010'",
                    "{ \"eq\" : [ { \"property\" : \"F_CODE\" }, \"AP010\" ] }",
                    "F_CODE LIKE 'AQ%'", "{\"and\":[{\"eq\":[{ \"property\" : \"F_CODE\" }, \"AP010\"]},{ \"before\":" +
                    "[{ \"property\" : \"ZI001_SDV\"},\"2013-01-01\"]}]}"
                ]
            }
        }

        Row {
            spacing: 8
            Text {
                id: count_text
                anchors.verticalCenter: parent.verticalCenter
                text: "Max Features"
            }

            TextField {
                id: count_field
                anchors.verticalCenter: parent.verticalCenter
                width: 200
                text: "1000"
                selectByMouse: true
                validator: IntValidator{}
            }
        }

//        Row {
//            spacing: 5
//            Text {
//                id: switch_text
//                anchors.verticalCenter: parent.verticalCenter
//                text: "Date Filter"
//                rightPadding: 16
//            }

//            Switch {
//                id: switch_button
//            }
//        }

        Row {
            spacing: 8
            Text {
                id: from_field
                anchors.verticalCenter: parent.verticalCenter
                text: "From"
                rightPadding: 40
            }

            TextField {
                id: from_date
                anchors.verticalCenter: parent.verticalCenter
                width: 200
                text: ""
                selectByMouse: true
                validator: RegExpValidator { regExp: /(0[1-9]|1[012])[-](0[1-9]|[12][0-9]|3[01])[-](19|20)\d\d/ }
                placeholderText: "MM-DD-YYYY"
            }
        }

        Row {
            spacing: 8
            Text {
                id: to_field
                anchors.verticalCenter: parent.verticalCenter
                text: "To"
                rightPadding: 53
            }

            TextField {
                id: to_date
                anchors.verticalCenter: parent.verticalCenter
                width: 200
                text: ""
                selectByMouse: true
                validator: RegExpValidator { regExp: /(0[1-9]|1[012])[-](0[1-9]|[12][0-9]|3[01])[-](19|20)\d\d/ }
                placeholderText: "MM-DD-YYYY"
            }
        }

        Button {
            anchors.horizontalCenter: parent.horizontalCenter
            text: "Query"
//            onClicked: model.query(clause_menu/* + count_field.text + from_date + to_date*/);
            onClicked:
            {
                var clause_text = clause_menu.currentText;
                var from_date_text = from_date.text;
                var to_date_text = to_date.text;

                var final_query = clause_text + from_date_text + to_date_text;
            }


        }
    }
}
