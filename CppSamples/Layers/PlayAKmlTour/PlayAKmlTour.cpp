// [WriteFile Name=PlayAKmlTour, Category=Layers]
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

#ifdef PCH_BUILD
#include "pch.hpp"
#endif // PCH_BUILD

// sample headers
#include "PlayAKmlTour.h"

// ArcGIS Maps SDK headers
#include "ArcGISTiledElevationSource.h"
#include "ElevationSourceListModel.h"
#include "Error.h"
#include "KmlContainer.h"
#include "KmlDataset.h"
#include "KmlLayer.h"
#include "KmlNodeListModel.h"
#include "KmlTour.h"
#include "KmlTourController.h"
#include "LayerListModel.h"
#include "MapTypes.h"
#include "Scene.h"
#include "SceneQuickView.h"
#include "Surface.h"

// Qt headers
#include <QStandardPaths>
#include <QtCore/qglobal.h>

using namespace Esri::ArcGISRuntime;

// helper method to get cross platform data path
namespace
{
  QString defaultDataPath()
  {
    QString dataPath;

  #ifdef Q_OS_IOS
    dataPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
  #else
    dataPath = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
  #endif

    return dataPath;
  }
} // namespace

PlayAKmlTour::PlayAKmlTour(QObject* parent /* = nullptr */):
  QObject(parent),
  m_scene(new Scene(BasemapStyle::ArcGISImageryStandard, this)),
  m_dataPath(defaultDataPath() + "/ArcGIS/Runtime/Data")
{
  // create a new elevation source from Terrain3D REST service
  ArcGISTiledElevationSource* elevationSource = new ArcGISTiledElevationSource(
        QUrl("https://elevation3d.arcgis.com/arcgis/rest/services/WorldElevation3D/Terrain3D/ImageServer"), this);

  // add the elevation source to the scene to display elevation
  m_scene->baseSurface()->elevationSources()->append(elevationSource);

  m_kmlDataset = new KmlDataset(QUrl::fromLocalFile(m_dataPath + "/kml/Esri_tour.kmz"), this);
  m_kmlLayer = new KmlLayer(m_kmlDataset, this);

  connect(m_kmlLayer, &KmlLayer::doneLoading, this, [this](const Error& e)
  {
    if (!e.isEmpty())
    {
      qDebug() << e.message();
      return;
    }

    m_scene->operationalLayers()->append(m_kmlLayer);

    m_kmlTour = findFirstKMLTour(m_kmlDataset->rootNodes());
    m_kmlTourController = new KmlTourController(this);

    if (m_kmlTour)
    {
      connect(m_kmlTour, &KmlTour::tourStatusChanged, this, [this](KmlTourStatus tourStatus)
      {
        switch (tourStatus) {
          case KmlTourStatus::Completed:
          case KmlTourStatus::Initialized:
            m_playButtonEnabled = true;
            m_pauseButtonEnabled = false;
            m_resetButtonEnabled = true;
            break;
          case KmlTourStatus::Playing:
            m_playButtonEnabled = false;
            m_pauseButtonEnabled = true;
            m_resetButtonEnabled = true;
            break;
          case KmlTourStatus::Paused:
            m_playButtonEnabled = true;
            m_pauseButtonEnabled = false;
            m_resetButtonEnabled = true;
            break;
          case KmlTourStatus::Initializing:
          case KmlTourStatus::NotInitialized:
            break;
        }

        emit playButtonEnabledChanged();
        emit pauseButtonEnabledChanged();
        emit resetButtonEnabledChanged();
      });

      m_kmlTourController->setTour(m_kmlTour);
    }
  });

  m_kmlLayer->load();
}

PlayAKmlTour::~PlayAKmlTour() = default;

void PlayAKmlTour::init()
{
  // Register classes for QML
  qmlRegisterType<SceneQuickView>("Esri.Samples", 1, 0, "SceneView");
  qmlRegisterType<PlayAKmlTour>("Esri.Samples", 1, 0, "PlayAKmlTourSample");
}

SceneQuickView* PlayAKmlTour::sceneView() const
{
  return m_sceneView;
}

// Set the view (created in QML)
void PlayAKmlTour::setSceneView(SceneQuickView* sceneView)
{
  if (!sceneView || sceneView == m_sceneView)
    return;

  m_sceneView = sceneView;
  m_sceneView->setArcGISScene(m_scene);

  emit sceneViewChanged();
}

void PlayAKmlTour::playKmlTour()
{
  m_kmlTourController->play();
}
void PlayAKmlTour::pauseKmlTour()
{
  m_kmlTourController->pause();
}
void PlayAKmlTour::resetKmlTour()
{
  m_kmlTourController->reset();
}

KmlTour* PlayAKmlTour::findFirstKMLTour(const QList<KmlNode*>& nodes)
{
  for (KmlNode* node : nodes)
  {
    if (node->kmlNodeType() == KmlNodeType::KmlTour)
      return dynamic_cast<KmlTour*>(node);
    else if ((node->kmlNodeType() == KmlNodeType::KmlDocument) || (node->kmlNodeType() == KmlNodeType::KmlFolder))
      return findFirstKMLTourFromListModel(*dynamic_cast<KmlContainer*>(node)->childNodesListModel());
  }
  return nullptr;
}

KmlTour* PlayAKmlTour::findFirstKMLTourFromListModel(const KmlNodeListModel& nodes)
{
  for (KmlNode* node : nodes)
  {
    if (node->kmlNodeType() == KmlNodeType::KmlTour)
      return dynamic_cast<KmlTour*>(node);
    else if ((node->kmlNodeType() == KmlNodeType::KmlDocument) || (node->kmlNodeType() == KmlNodeType::KmlFolder))
      return findFirstKMLTourFromListModel(*dynamic_cast<KmlContainer*>(node)->childNodesListModel());
  }
  return nullptr;
}
