// [WriteFile Name=DisplayContentOfUtilityNetworkContainer, Category=UtilityNetwork]
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

Item {
    id: item1

    // Declare the C++ instance which creates the scene etc. and supply the view
    DisplayContentOfUtilityNetworkContainerSample {
        id: model
        mapView: view

    }

    // add a mapView component
    MapView {
        id: view
        anchors.fill: parent

        Component.onCompleted: {
            // Set and keep the focus on SceneView to enable keyboard navigation
            forceActiveFocus();
        }
    }

    Button {
        id: containerCloseButton
        text: "Close container view"
        anchors.bottom: parent.bottom
        focusPolicy: Qt.NoFocus
        anchors.bottomMargin: 20
        anchors.horizontalCenter: parent.horizontalCenter
        //visible: model.showContainerView
        onClicked: {
            model.showContainerView = false;
        }
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
