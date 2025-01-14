// [WriteFile Name=FindPlace, Category=Search]
// [Legal]
// Copyright 2017 Esri.
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

Rectangle {
    property bool pressed: false
    signal buttonClicked()

    width: 175
    height: 35
    color: pressed ? "#959595" : "#D6D6D6"
    radius: 5
    border {
        color: "#585858"
        width: 1
    }

    Text {
        anchors.centerIn: parent
        text: "Redo search in this area"
        font.pixelSize: 14
        color: "#474747"
    }

    MouseArea {
        anchors.fill: parent
        onPressed: searchExtentButton.pressed = true
        onReleased: searchExtentButton.pressed = false
        onClicked: {
            buttonClicked();
        }
    }
}
