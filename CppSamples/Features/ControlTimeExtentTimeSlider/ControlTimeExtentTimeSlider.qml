// [WriteFile Name=ControlTimeExtentTimeSlider, Category=Features]
// [Legal]
// Copyright 2021 Esri.
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
import Esri.ArcGISRuntime.Toolkit
import Esri.Samples

Item {

    // add a mapView component
    MapView {
        id: view
        anchors.fill: parent
        focus: true

        // Add a TimeSlider from the toolkit to the MapView
        TimeSlider {
            anchors {
                left: parent.left
                right: parent.right
                bottom: parent.bottom
            }

            geoView: view
        }
    }

    // Declare the C++ instance which creates the map etc. and supply the view
    ControlTimeExtentTimeSliderSample {
        id: model
        mapView: view
    }
}
