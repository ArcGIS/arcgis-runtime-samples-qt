// [WriteFile Name=ShowOrgBasemaps, Category=CloudAndPortal]
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
import Esri.ArcGISRuntime 100.0
import Esri.Samples 1.0
import Esri.ArcGISExtras 1.1
import Esri.ArcGISRuntime.Toolkit.Dialogs 2.0

ShowOrgBasemapsSample {
    width: 800
    height: 600
    clip: true

    property real scaleFactor: System.displayScaleFactor

    onPortalLoadedChanged: { gridFadeIn.running = true; }

    BusyIndicator {
        anchors.centerIn: parent
        running: !mapView.visible && !portalLoaded
    }

    Text{
        id: title
        anchors {
            top: parent.top;
            left: parent.left;
            right: parent.right;
            margins: 10
        }
        font.pointSize: 14
        font.bold: true
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignTop
        text: orgName.length > 0 ? orgName + " Basemaps" : "Loading Organization Basemaps..."
        wrapMode: Text.Wrap
        elide: Text.ElideRight
    }

    MapView {
        id: mapView
        objectName: "mapView"
        anchors {
            top: title.bottom;
            bottom: parent.bottom;
            left: parent.left;
            right: parent.right
        }
        visible: false
    }

    GridView {
        id: basemapsGrid
        anchors {
            top: title.bottom;
            bottom: parent.bottom;
            left: parent.left;
            right: parent.right
        }
        visible: portalLoaded
        cellWidth: 128 * scaleFactor;
        cellHeight: 128 * scaleFactor
        opacity: 0
        focus: true
        clip: true
        model: basemaps

        delegate: Rectangle {
            anchors.margins: 5 * scaleFactor
            width: basemapsGrid.cellWidth
            height: width
            border {width: 2; color: index === basemapsGrid.currentIndex ? "blue" : "lightgrey"}
            color: index === basemapsGrid.currentIndex ? "yellow" : "white"
            radius: 2
            clip: true

            Image {
                id: basemapImg
                anchors {
                    bottom: basemapLabel.top;
                    horizontalCenter: parent.horizontalCenter
                }
                height: parent.height - ( basemapLabel.height * 2 );
                source: thumbnailUrl
                width: height
                fillMode: Image.PreserveAspectCrop
            }

            Text {
                id: basemapLabel
                anchors {
                    bottom: parent.bottom;
                    left: parent.left;
                    right: parent.right
                }
                height: 16 * scaleFactor
                z: 100
                wrapMode: Text.Wrap
                elide: Text.ElideRight
                text: title
                font.pointSize: 8
                font.bold: index === basemapsGrid.currentIndex
            }

            MouseArea {
                enabled: !mapView.visible && portalLoaded
                anchors.fill: parent

                onClicked: {
                    if (!enabled)
                        return;

                    basemapsGrid.currentIndex = index;
                }
                onDoubleClicked: {
                    if (!enabled)
                        return;

                    selectedAnimation.running = true;
                    loadSelectedBasemap(index);
                    gridFadeOut.running = true;
                }
            }

            SequentialAnimation on opacity {
                id: selectedAnimation
                running: false
                loops: 4
                PropertyAnimation { to: 0; duration: 60 }
                PropertyAnimation { to: 1; duration: 60 }
            }
        }

        OpacityAnimator on opacity {
            id: gridFadeIn
            from: 0;
            to: 1;
            duration: 2000
            running: false
        }

        OpacityAnimator on opacity {
            id: gridFadeOut
            from: 1;
            to: 0;
            duration: 2000
            running: false
        }
    }

    AuthenticationView {
        authenticationManager: authManager
    }

    // Neatline rectangle
    Rectangle {
        anchors.fill: parent
        color: "transparent"
        border {
            width: 0.5 * scaleFactor
            color: "black"
        }
    }
}
