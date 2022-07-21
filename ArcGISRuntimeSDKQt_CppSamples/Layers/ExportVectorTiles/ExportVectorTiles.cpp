// [WriteFile Name=ExportVectorTiles, Category=Layers]
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

#ifdef PCH_BUILD
#include "pch.hpp"
#endif // PCH_BUILD

#include "ExportVectorTiles.h"

#include "ArcGISVectorTiledLayer.h"
#include "ExportVectorTilesTask.h"
#include "ExportVectorTilesParameters.h"
#include "GeometryEngine.h"
#include "GraphicsOverlay.h"
#include "Map.h"
#include "MapQuickView.h"
#include "Symbol.h"
#include "SimpleLineSymbol.h"

#include <QTemporaryDir>

using namespace Esri::ArcGISRuntime;

ExportVectorTiles::ExportVectorTiles(QObject* parent /* = nullptr */):
  QObject(parent),
  m_map(new Map(BasemapStyle::ArcGISStreetsNight, this))
{
}

ExportVectorTiles::~ExportVectorTiles() = default;

void ExportVectorTiles::init()
{
  // Register the map view for QML
  qmlRegisterType<MapQuickView>("Esri.Samples", 1, 0, "MapView");
  qmlRegisterType<ExportVectorTiles>("Esri.Samples", 1, 0, "ExportVectorTilesSample");
}

MapQuickView* ExportVectorTiles::mapView() const
{
  return m_mapView;
}

// Set the view (created in QML)
void ExportVectorTiles::setMapView(MapQuickView* mapView)
{
  if (!mapView || mapView == m_mapView)
    return;

  m_mapView = mapView;
  m_mapView->setMap(m_map);
  m_mapView->setViewpoint(Viewpoint(34.049, -117.181, 1e4));

  m_graphicsOverlay = new GraphicsOverlay(this);
  m_mapView->graphicsOverlays()->append(m_graphicsOverlay);

  m_downloadAreaGraphic = new Graphic(this);
  m_downloadAreaGraphic->setSymbol(new SimpleLineSymbol(SimpleLineSymbolStyle::Solid, Qt::green, 3, this));

  m_graphicsOverlay->graphics()->append(m_downloadAreaGraphic);

  emit mapViewChanged();
}

void ExportVectorTiles::startExport(double xSW, double ySW, double xNE, double yNE)
{
  if (m_map->basemap()->baseLayers()->first()->layerType() != LayerType::ArcGISVectorTiledLayer)
    return;

  ArcGISVectorTiledLayer* vectorTiledLayer = static_cast<ArcGISVectorTiledLayer*>(m_map->basemap()->baseLayers()->first());
  ExportVectorTilesTask* exportTask = new ExportVectorTilesTask(vectorTiledLayer->url(), this);

  // Create an envelope from the QML rectangle corners
  const Point corner1 = m_mapView->screenToLocation(xSW, ySW);
  const Point corner2 = m_mapView->screenToLocation(xNE, yNE);
  const Envelope extent = Envelope(corner1, corner2);
  const Envelope exportArea = GeometryEngine::project(extent, vectorTiledLayer->spatialReference());

  m_downloadAreaGraphic->setGeometry(exportArea);

  const QString vectorTileCachePath = m_tempDir.path() + ("/vectorTiles.vtpk");
  const QString itemResourcePath = m_tempDir.path() + "/itemResources";

  connect(exportTask, &ExportVectorTilesTask::createDefaultExportVectorTilesParametersCompleted, this,
          [exportTask, vectorTileCachePath, itemResourcePath, this](QUuid, ExportVectorTilesParameters exportParameters)
  {
    exportParameters.setEsriVectorTilesDownloadOption(EsriVectorTilesDownloadOption::UseReducedFontsService);

    m_exportJob = exportTask->exportVectorTiles(exportParameters, vectorTileCachePath, itemResourcePath);

    connect(m_exportJob, &ExportVectorTilesJob::jobDone, this, [this]()
    {
      if (m_exportJob->error().isEmpty())
      {
        VectorTileCache* vectorTileCache = m_exportJob->result()->vectorTileCache();
        ItemResourceCache* itemResourceCache = m_exportJob->result()->itemResourceCache();

        ArcGISVectorTiledLayer* downloadedLayer = new ArcGISVectorTiledLayer(vectorTileCache, itemResourceCache, this);
        m_map->setBasemap(new Basemap(downloadedLayer, this));
      }
    });

    connect(m_exportJob, &Job::progressChanged, this, [this]()
    {
      m_downloadProgress = m_exportJob->progress();
      emit downloadProgressChanged();
    });

    m_exportJob->start();
  });

  exportTask->createDefaultExportVectorTilesParameters(exportArea, m_mapView->mapScale() * 0.1);
}

int ExportVectorTiles::downloadProgress() const
{
  return m_downloadProgress;
}

void ExportVectorTiles::cancelExport()
{
  m_exportJob->cancel();
}