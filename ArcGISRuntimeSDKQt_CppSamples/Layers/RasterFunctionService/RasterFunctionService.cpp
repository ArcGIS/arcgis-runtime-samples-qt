// [WriteFile Name=RasterFunctionService, Category=Layers]
// [Legal]
// Copyright 2017 Esri.

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

#include "RasterFunctionService.h"

#include "Map.h"
#include "MapQuickView.h"
#include "RasterLayer.h"
#include "Basemap.h"
#include "ArcGISTiledLayer.h"
#include "ImageServiceRaster.h"
#include "RasterFunction.h"
#include "ArcGISRuntimeEnvironment.h"

#include <QQmlProperty>

using namespace Esri::ArcGISRuntime;

RasterFunctionService::RasterFunctionService(QQuickItem* parent /* = nullptr */):
  QQuickItem(parent)
{
}

RasterFunctionService::~RasterFunctionService()
{
}

void RasterFunctionService::init()
{
  // Register the map view for QML
  qmlRegisterType<MapQuickView>("Esri.Samples", 1, 0, "MapView");
  qmlRegisterType<RasterFunctionService>("Esri.Samples", 1, 0, "RasterFunctionServiceSample");
}

void RasterFunctionService::componentComplete()
{
  QQuickItem::componentComplete();

  ArcGISRuntimeEnvironment::setLicense("runtimestandard,1000,rud549870137,17-oct-2017,HC4XTK8EL9ELCG5AD254");
  m_dataPath = QUrl(QQmlProperty::read(this, "dataPath").toString()).toLocalFile();

  // find QML MapView component
  m_mapView = findChild<MapQuickView*>("mapView");
  m_mapView->setWrapAroundMode(WrapAroundMode::Disabled);

  // create a new tiled layer to add a basemap
  m_map = new Map(Basemap::streets(this), this);
  m_mapView->setMap(m_map);

  // create an image service raster
  m_imageServiceRaster = new ImageServiceRaster(QUrl("http://sampleserver6.arcgisonline.com/arcgis/rest/services/NLCDLandCover2001/ImageServer"), this);
  // zoom to the raster's extent once it's loaded
  connect(m_imageServiceRaster, &ImageServiceRaster::doneLoading, this, [this]()
  {
      m_mapView->setViewpoint(Viewpoint(m_imageServiceRaster->serviceInfo().fullExtent()));
  });

  // create a raster layer using the image service raster
  m_rasterLayer = new RasterLayer(m_imageServiceRaster, this);
  // add the raster layer to the map's operational layers
  m_map->operationalLayers()->append(m_rasterLayer);
}

void RasterFunctionService::applyRasterFunction()
{
  RasterFunction* rasterFunction = new RasterFunction(m_dataPath + "/hillshade_simplified.json");
  RasterFunctionArguments* arguments = rasterFunction->arguments();
  arguments->setRaster("raster", m_imageServiceRaster);

  // create a new raster with the raster function and add to the operational layer
  Raster* raster = new Raster(rasterFunction, this);
  RasterLayer* rasterLayer = new RasterLayer(raster, this);
  m_map->operationalLayers()->clear();
  m_map->operationalLayers()->append(rasterLayer);
}
