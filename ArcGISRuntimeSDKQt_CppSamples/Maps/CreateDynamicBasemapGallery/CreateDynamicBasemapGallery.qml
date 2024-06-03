// [WriteFile Name=CreateDynamicBasemapGallery, Category=Maps]
// [Legal]
// Copyright 2024 Esri.

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

import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts
import Esri.Samples

Item {
    Row {
        anchors.fill: parent

        Pane {
            id: basemapGallery
            width: 220
            height: parent.height

            contentItem: ColumnLayout {
                GridView {
                    id: basemapView
                    Layout.alignment: Qt.AlignTop
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    clip: true

                    width: parent.width
                    model: model.gallery
                    cellWidth: 200
                    cellHeight: 160
                    highlight: Rectangle {
                        width: basemapView.cellWidth;
                        height: basemapView.cellHeight
                        color: "lightsteelblue";
                        anchors.fill: basemapView.currentItem
                    }
                    highlightFollowsCurrentItem: false;

                    delegate: ItemDelegate {
                        id: basemapDelegate
                        width: basemapView.cellWidth
                        height: basemapView.cellHeight
                        required property string styleName
                        required property string previewImageUrl
                        onClicked: {
                            model.updateSelectedStyle(styleName);
                            basemapView.currentIndex = model.indexOfSelectedStyle();
                        }
                        Column {
                            spacing: 5
                            padding: 10
                            Text {
                                font.bold: true
                                font.italic: true
                                font.underline: true
                                text: styleName + ":"
                            }
                            Image {
                                source: previewImageUrl
                                fillMode: Image.PreserveAspectFit
                                width: basemapView.cellWidth - 25
                            }
                        }
                    }
                }
                Text {
                    Layout.preferredHeight: 15
                    font.underline: true
                    text: "Language Strategy:"
                }
                ComboBox {
                    id: languageStrategyOptions
                    Layout.fillWidth: true
                    Layout.preferredHeight: 30
                    model: model.languageStrategies
                    enabled: model.languageStrategies.length !== 0
                }
                Text {
                    Layout.preferredHeight: 15
                    font.underline: true
                    text: "Language:"
                }
                ComboBox {
                    id: languages
                    Layout.fillWidth: true
                    Layout.preferredHeight: 30
                    model: model.languages
                    enabled: model.languages.length !== 0
                }
                Text {
                    Layout.preferredHeight: 15
                    font.underline: true
                    text: "Worldview:"
                }
                ComboBox {
                    id: worldviews
                    Layout.fillWidth: true
                    Layout.preferredHeight: 30
                    model: model.worldviews
                    enabled: model.worldviews.length !== 0
                }
                Button {
                    height: 50
                    text: "Load"
                    onPressed: model.loadBasemap(
                                   languageStrategyOptions.currentText,
                                   languages.currentText,
                                   worldviews.currentText);
                }
            }
        }

        // Create MapQuickView here, and create its Map etc. in C++ code
        MapView {
            id: view
            width: parent.width - 220
            height: parent.height
            // set focus to enable keyboard navigation
            focus: true
        }
    }

    // Declare the C++ instance which creates the map etc. and supply the view
    CreateDynamicBasemapGallerySample {
        id: model
        mapView: view
    }
}