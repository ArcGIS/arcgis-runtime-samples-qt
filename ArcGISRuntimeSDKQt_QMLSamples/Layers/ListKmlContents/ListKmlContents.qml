// [WriteFile Name=ListKmlContents, Category=Layers]
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
import QtQuick.Layouts 1.11
import Esri.ArcGISRuntime 100.9
import Esri.ArcGISExtras 1.1

Rectangle {
    id: rootRectangle
    clip: true
    width: 800
    height: 600

    readonly property url dataPath: System.userHomePath + "/ArcGIS/Runtime/Data/kml/"
    property var nodesOnLevel: []
    property var kmlNodesList: []
    property var currentNode: null
    property string labelText: ""
    property bool topLevel: true

    // recursively build list of nodes
    function buildTree(parentNode) {
        let childNodes = parentNode.childNodesListModel;
        if (childNodes !== undefined && childNodes !== null) {
            for (let i = 0; i < childNodes.count; i++) {
                let node = childNodes.get(i);
                node.visible = true;
                kmlNodesList.push(node);

                buildTree(node);
            }
        }
    }

    // recursively build string to indicate node's ancestors
    function buildPathLabel(node) {
        if (node.parentNode !== undefined && node.parentNode !== null) {
            buildPathLabel(node.parentNode);
            labelText = labelText.concat(">");
        }
        labelText = labelText.concat(node.name);
    }

    function displayChildren(parentNode) {
        // if node has children, then display children
        if (parentNode.childNodesListModel !== null && parentNode.childNodesListModel !== undefined) {
            let childNodes = parentNode.childNodesListModel
            let lastLevel = true;

            // clear previous node names
            nodesOnLevel = [];
            for (let i = 0; i < childNodes.count; i++) {
                let node = childNodes.get(i);
                nodesOnLevel.push(node.name + getKmlNodeType(node));

                // check if on last level of nodes
                if (node.childNodesListModel !== undefined && node.childNodesListModel !== null) {
                    nodesOnLevel[i] = nodesOnLevel[i].concat(" >"); // indicate there are children
                    lastLevel = false;
                }
            }
            myListView.model = nodesOnLevel;

            if (lastLevel) {
                currentNode = childNodes.get(0);
            }
        }
    }

    // display selected node on sceneview and show its children
    function processSelectedNode(nodeName) {
        // extract the node name from string, formatted "name - nodeType"
        let ind = nodeName.lastIndexOf(" - ");
        if (ind > -1) {
            nodeName = nodeName.substring(0, ind);
        }

        // find node with matching name
        for (let i = 0; i < kmlNodesList.length; i++) {
            if (nodeName === kmlNodesList[i].name) {
                topLevel = false;
                let node = kmlNodesList[i];
                currentNode = node;

                // set the viewpoint to the extent of the selected node
                let nodeExtent = node.extent;
                if (!nodeExtent.empty) {
                    sceneView.setViewpoint(ArcGISRuntimeEnvironment.createObject("ViewpointExtent", {extent: nodeExtent}))
                }

                // update path label
                labelText = "";
                buildPathLabel(node);

                // show the children of the node
                displayChildren(node);
                break;
            }
        }
    }

    // returns string containing the KmlNodeType
    function getKmlNodeType(node) {
        let type = "";
        switch(node.kmlNodeType) {
        case Enums.KmlNodeTypeKmlDocument:
            type = "KmlDocument";
            break;
        case Enums.KmlNodeTypeKmlFolder:
            type = "KmlFolder";
            break;
        case Enums.KmlNodeTypeKmlGroundOverlay:
            type = "KmlGroundOverlay";
            break;
        case Enums.KmlNodeTypeKmlNetworkLink:
            type = "KmlNetworkLink";
            break;
        case Enums.KmlNodeTypeKmlPhotoOverlay:
            type = "KmlPhotoOverlay";
            break;
        case Enums.KmlNodeTypeKmlPlacemark:
            type = "KmlPlacemark";
            break;
        case Enums.KmlNodeTypeKmlScreenOverlay:
            type = "KmlScreenOverlay";
            break;
        case Enums.KmlNodeTypeKmlTour:
            type = "KmlTour";
            break;
        }
        return " - " + type;
    }

    SceneView {
        id: sceneView
        anchors.fill: parent

        // create window for displaying the KML contents
        Control {
            width: 300
            background : Rectangle {
                color: "lightgrey"
            }
            contentItem: GridLayout {
                columns: 2

                Button {
                    id: backButton
                    Layout.margins: 3
                    text: "<"
                    enabled: !topLevel
                    flat: true
                    highlighted: pressed
                    onClicked: {
                        // display previous level of nodes
                        let parentNode = currentNode.parentNode;
                        let grandparentNode = parentNode.parentNode;

                        if (grandparentNode !== undefined && grandparentNode !== null) {
                            labelText = "";
                            buildPathLabel(grandparentNode);

                            displayChildren(grandparentNode);
                            currentNode = grandparentNode;
                        }
                        // if parent node is undefined, then at top of tree
                        else {
                            labelText = "";
                            buildPathLabel(parentNode);

                            displayChildren(parentNode);
                            topLevel = true;
                        }

                        if (currentNode.name === "") {
                            topLevel = true;
                        }
                    }
                }

                Text {
                    Layout.fillWidth: true
                    id: textLabel
                    text: labelText
                    wrapMode: Text.Wrap
                }

                ListView {
                    id: myListView
                    Layout.columnSpan: 2
                    Layout.fillWidth: true
                    model: nodesOnLevel
                    Layout.preferredHeight: contentHeight
                    delegate: Component {
                        Button {
                            text: modelData
                            anchors {
                                left: parent.left
                                right: parent.right
                            }
                            onClicked: {
                                processSelectedNode(text);
                            }
                            highlighted: pressed
                        }
                    }
                }
            }
        }

        Scene {
            id: scene
            BasemapImageryWithLabels {}

            Surface {
                ArcGISTiledElevationSource {
                    url: "http://elevation3d.arcgis.com/arcgis/rest/services/WorldElevation3D/Terrain3D/ImageServer"
                }
            }

            KmlLayer {
                KmlDataset {
                    id: kmlDataset
                    url: dataPath + "esri_test_data.kmz"

                    onLoadStatusChanged: {
                        if (loadStatus !== Enums.LoadStatusLoaded)
                            return;

                        for (let i = 0; i < rootNodes.length; i++) {
                            nodesOnLevel.push(rootNodes[i].name);
                            kmlNodesList.push(rootNodes[i]);

                            buildTree(rootNodes[i]);
                        }

                        // if at top node, then display children
                        if (kmlNodesList.length !== 0 && kmlNodesList[0].parentNode === null) {
                            displayChildren(kmlNodesList[0]);
                        }
                    }
                }
            }
        }
    }
}
