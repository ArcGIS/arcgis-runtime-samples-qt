// [Legal]
// Copyright 2022 Esri.

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

import QtQuick 2.5
import QtQuick.Controls 2.2
import Qt.labs.platform
import QtQuick.Layouts 1.2
import Esri.ArcGISRuntimeSamples 1.0
import Esri.ArcGISExtras 1.1

Page {
    id: manageOfflineDataViewPage
    visible: SampleManager.currentMode === SampleManager.ManageOfflineDataView
    property bool manageOfflineDataViewDownloadInProgress: false

    FileFolder {
        id: fileFolder
    }

    ColumnLayout {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter

        Image {
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            source: "qrc:/logo.png"
            clip: true
        }

        Text {
            id: name
            text: qsTr("Currently downloading offline data.")
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            font {
                family: fontFamily
                weight: Font.DemiBold
            }
            Layout.fillWidth: true
            clip: true
            visible: SampleManager.downloadInProgress && SampleManager.currentMode
        }

        Button {
            text: qsTr("Download all offline data")
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            visible: !SampleManager.downloadInProgress
            onClicked: {
                if (System.isOnline) {
                    allDataDownloadLoader.item.downloadAllDataItems();
                    manageOfflineDataViewDownloadInProgress = true;
                } else {
                    SampleManager.currentMode = SampleManager.NetworkRequiredView;
                }
            }
            clip: true
        }

        Button {
            text: qsTr("Delete all offline data")
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            visible: !SampleManager.downloadInProgress
            onClicked: messageDialog.visible = fileFolder.removeFolder(System.userHomePath + "/ArcGIS/Runtime/Data", true);
            clip: true
        }

        Label {
            text: SampleManager.downloadText
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            Layout.fillWidth: true
            horizontalAlignment: Label.AlignHCenter
            visible: SampleManager.downloadInProgress || (allDataDownloadLoader.item && allDataDownloadLoader.item.failedToDownload)
            font {
                family: fontFamily
            }
            clip: true
        }

        Label {
            text: "%1% complete".arg(SampleManager.downloadProgress)
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            Layout.fillWidth: true
            horizontalAlignment: Text.AlignHCenter
            visible: SampleManager.downloadInProgress
            font {
                family: fontFamily
            }
            clip: true
        }

        ProgressBar {
            Layout.fillHeight: false
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            Layout.fillWidth: true
            from: 0
            to: 100
            value: SampleManager.downloadProgress
            visible: SampleManager.downloadInProgress
            clip: true
        }
    }

    MessageDialog {
        id: messageDialog
        title: "Delete all offline data"
        text: "Delete all offline data was successful"
        visible: false
        onRejected: {
            visible = false;
        }
    }

    Loader {
        id: allDataDownloadLoader
        asynchronous: true
        visible: status == Loader.Ready
        sourceComponent: manageOfflineDataViewPage.visible || manageOfflineDataViewDownloadInProgress ? allDataDownloaderComponent : undefined
    }

    Component {
        id: allDataDownloaderComponent
        DataDownloader {
        }
    }

    Connections {
        target: SampleManager

        function onDoneDownloadingChanged () {
            manageOfflineDataViewDownloadInProgress = false;
        }
    }
}

