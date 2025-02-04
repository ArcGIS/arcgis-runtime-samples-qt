// [WriteFile Name=IdentifyLayers, Category=Maps]
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

#ifdef PCH_BUILD
#include "pch.hpp"
#endif // PCH_BUILD

// sample headers
#include "IdentifyLayers.h"

// ArcGIS Maps SDK headers
#include "ArcGISMapImageLayer.h"
#include "ArcGISSublayer.h"
#include "ArcGISSublayerListModel.h"
#include "Basemap.h"
#include "Error.h"
#include "FeatureLayer.h"
#include "IdentifyLayerResult.h"
#include "LayerListModel.h"
#include "Map.h"
#include "MapQuickView.h"
#include "MapTypes.h"
#include "Point.h"
#include "ServiceFeatureTable.h"
#include "SpatialReference.h"
#include "Viewpoint.h"

// Qt headers
#include <QFuture>
#include <QQueue>
#include <QUuid>

using namespace Esri::ArcGISRuntime;

IdentifyLayers::IdentifyLayers(QQuickItem* parent /* = nullptr */):
  QQuickItem(parent)
{
}

void IdentifyLayers::init()
{
  // Register the map view for QML
  qmlRegisterType<MapQuickView>("Esri.Samples", 1, 0, "MapView");
  qmlRegisterType<IdentifyLayers>("Esri.Samples", 1, 0, "IdentifyLayersSample");
}

void IdentifyLayers::componentComplete()
{
  QQuickItem::componentComplete();

  // find QML MapView component
  m_mapView = findChild<MapQuickView*>("mapView");

  // Create a map using the topographic basemap
  m_map = new Map(BasemapStyle::ArcGISTopographic, this);

  // add a map image layer
  QUrl mapServiceUrl("https://sampleserver6.arcgisonline.com/arcgis/rest/services/SampleWorldCities/MapServer");
  ArcGISMapImageLayer* imageLayer = new ArcGISMapImageLayer(mapServiceUrl, this);
  m_map->operationalLayers()->append(imageLayer);

  // hide 2 of the sublayers in the map image layer
  connect(imageLayer, &ArcGISMapImageLayer::doneLoading, this, [imageLayer](const Error& e)
  {
    if (!e.isEmpty())
    {
      qDebug() << e.message() << e.additionalMessage();
      return;
    }

    imageLayer->mapImageSublayers()->at(1)->setVisible(false);
    imageLayer->mapImageSublayers()->at(2)->setVisible(false);
  });

  // add a feature layer
  QUrl featureLayerUrl("https://sampleserver6.arcgisonline.com/arcgis/rest/services/DamageAssessment/FeatureServer/0");
  ServiceFeatureTable* featureTable = new ServiceFeatureTable(featureLayerUrl, this);
  FeatureLayer* featureLayer = new FeatureLayer(featureTable, this);
  m_map->operationalLayers()->append(featureLayer);

  // set an initial viewpoint
  const double mapScale = 68015210;
  const double x = -10977012.785807;
  const double y = 4514257.550369;
  const Point pt(x, y, SpatialReference::webMercator());
  Viewpoint initialViewpoint(pt, mapScale);
  m_map->setInitialViewpoint(initialViewpoint);

  // Set map to map view
  m_mapView->setMap(m_map);

  // connect signals
  connectSignals();
}

void IdentifyLayers::connectSignals()
{
  // identify layers on mouse click
  connect(m_mapView, &MapQuickView::mouseClicked, this, [this](QMouseEvent& mouseEvent)
  {
    const double tolerance = 12.0;
    const bool returnPopups = false;
    const int maxResults = 10;

    m_mapView->identifyLayersAsync(mouseEvent.position(), tolerance, returnPopups, maxResults).then(this,
    [this](const QList<IdentifyLayerResult*>& results)
    {
      // reset the message text
      m_message = QString();

      for (IdentifyLayerResult* result : results)
      {
        // lambda for calculating result count
        auto geoElementsCountFromResult = [] (IdentifyLayerResult* result) -> int
        {
          // create temp list
          QList<IdentifyLayerResult*> tempResults{result};

          // use Depth First Search approach to handle recursion
          int count = 0;
          int index = 0;

          while (index < tempResults.length())
          {
            //get the result object from the array
            IdentifyLayerResult* identifyResult = tempResults[index];

            // update count with geoElements from the result
            count += identifyResult->geoElements().length();

            // check if the result has any sublayer results
            // if yes then add those result objects in the tempResults
            // array after the current result
            if (identifyResult->sublayerResults().length() > 0)
            {
              tempResults.append(identifyResult->sublayerResults().at(index));
            }

            // update the count and repeat
            index += 1;
          }
          return count;
        };

        const int count = geoElementsCountFromResult(result);
        QString layerName = result->layerContent()->name();
        m_message += QString("%1 : %2").arg(layerName).arg(count);
        m_message += "\n";
      }
      //remove last newline character
      if (!m_message.isEmpty())
        m_message.chop(1);

      emit messageChanged();
      emit showMessage();
      qDeleteAll(results);
    });

  });
}
