// [WriteFile Name=UpdateAttributesFeatureService, Category=EditData]
// [Legal]
// Copyright 2016 Esri.
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
#include "UpdateAttributesFeatureService.h"

// ArcGIS Maps SDK headers
#include "ArcGISFeature.h"
#include "AttributeListModel.h"
#include "Basemap.h"
#include "CalloutData.h"
#include "Envelope.h"
#include "Feature.h"
#include "FeatureEditResult.h"
#include "FeatureIterator.h"
#include "FeatureLayer.h"
#include "FeatureQueryResult.h"
#include "IdentifyLayerResult.h"
#include "LayerListModel.h"
#include "Map.h"
#include "MapQuickView.h"
#include "MapTypes.h"
#include "MapViewTypes.h"
#include "Point.h"
#include "QueryParameters.h"
#include "ServiceFeatureTable.h"
#include "SpatialReference.h"
#include "Viewpoint.h"

// Qt headers
#include <QFuture>
#include <QMouseEvent>
#include <QUrl>
#include <QUuid>

using namespace Esri::ArcGISRuntime;

namespace
{
  // Convenience RAII struct that deletes all pointers in given container.
  struct FeatureEditListResultLock
  {
    FeatureEditListResultLock(const QList<FeatureEditResult*>& list) : results(list) { }
    ~FeatureEditListResultLock() { qDeleteAll(results); }
    const QList<FeatureEditResult*>& results;
  };
}

UpdateAttributesFeatureService::UpdateAttributesFeatureService(QQuickItem* parent) :
  QQuickItem(parent)
{
}

UpdateAttributesFeatureService::~UpdateAttributesFeatureService() = default;

void UpdateAttributesFeatureService::init()
{
  qmlRegisterType<MapQuickView>("Esri.Samples", 1, 0, "MapView");
  qmlRegisterType<UpdateAttributesFeatureService>("Esri.Samples", 1, 0, "UpdateAttributesFeatureServiceSample");
  qmlRegisterUncreatableType<CalloutData>("Esri.Samples", 1, 0, "CalloutData", "CalloutData is an uncreatable type");
}

void UpdateAttributesFeatureService::componentComplete()
{
  QQuickItem::componentComplete();

  // find QML MapView component
  m_mapView = findChild<MapQuickView*>("mapView");
  m_mapView->setWrapAroundMode(WrapAroundMode::Disabled);

  // create a Map by passing in the Basemap
  m_map = new Map(BasemapStyle::ArcGISStreets, this);
  m_map->setInitialViewpoint(Viewpoint(Point(-10800000, 4500000, SpatialReference(102100)), 3e7));

  // set map on the map view
  m_mapView->setMap(m_map);

  // create the ServiceFeatureTable
  m_featureTable = new ServiceFeatureTable(QUrl("https://sampleserver6.arcgisonline.com/arcgis/rest/services/DamageAssessment/FeatureServer/0"), this);

  // create the FeatureLayer with the ServiceFeatureTable and add it to the Map
  m_featureLayer = new FeatureLayer(m_featureTable, this);
  m_map->operationalLayers()->append(m_featureLayer);

  connectSignals();
}

void UpdateAttributesFeatureService::connectSignals()
{
  // connect to the mouse clicked signal on the MapQuickView
  connect(m_mapView, &MapQuickView::mouseClicked, this, [this](QMouseEvent& mouseEvent)
  {
    // first clear the selection
    m_featureLayer->clearSelection();

    // set the properties for qml
    emit hideWindow();

    // call identify on the map view
    m_mapView->identifyLayerAsync(m_featureLayer, mouseEvent.position(), 5, false, 1).then(this, [this](IdentifyLayerResult* identifyResult)
    {
      onIdentifyLayerCompleted_(identifyResult);
    });
  });

  // connect to the viewpoint changed signal on the MapQuickView
  connect(m_mapView, &MapQuickView::viewpointChanged, this, [this]()
  {
    m_featureLayer->clearSelection();
    emit hideWindow();
  });
}

void UpdateAttributesFeatureService::updateSelectedFeature(QString fieldVal)
{
  // If the last connection is still hanging around we want to ensure it is disconnected.
  disconnect(m_featureLoadStatusChangedConnection);

  // connect to load status changed signal, remember the connection so we can kill it once
  // the slot has invoked.
  m_featureLoadStatusChangedConnection =
      connect(
          m_selectedFeature, &ArcGISFeature::loadStatusChanged,
          this, [this, fieldVal](LoadStatus)
                {
                  if (m_selectedFeature->loadStatus() == LoadStatus::Loaded)
                  {
                    // The conenction is invoked so we now forget all about this connection after this point.
                    disconnect(m_featureLoadStatusChangedConnection);

                    // update the select feature's attribute value
                    m_selectedFeature->attributes()->replaceAttribute("typdamage", fieldVal);

                    // update the feature in the feature table
                    m_featureTable->updateFeatureAsync(m_selectedFeature).then(this, [this]()
                    {
                      m_featureTable->applyEditsAsync().then(this, [this](const QList<FeatureEditResult*>& featureEditResults)
                      {
                        onApplyEditsCompleted_(featureEditResults);
                      });
                    });
                  }
                }
  );

  // load selecte feature
  m_selectedFeature->load();
}

QString UpdateAttributesFeatureService::featureType() const
{
  return m_featureType;
}

void UpdateAttributesFeatureService::onIdentifyLayerCompleted_(IdentifyLayerResult* identifyResult)
{
  if (!identifyResult)
    return;

  if (!identifyResult->geoElements().empty())
  {
    // select the item in the result
    m_featureLayer->selectFeature(static_cast<Feature*>(identifyResult->geoElements().at(0)));
    // Update the parent so the featureLayer is not deleted when the identifyResult is deleted.
    m_featureLayer->setParent(this);

    // obtain the selected feature with attributes
    QueryParameters queryParams;
    QString whereClause = "objectid=" + identifyResult->geoElements().at(0)->attributes()->attributeValue("objectid").toString();
    queryParams.setWhereClause(whereClause);
    m_featureTable->queryFeaturesAsync(queryParams).then(this, [this](FeatureQueryResult* featureQueryResult)
    {
      onQueryFeaturesCompleted_(featureQueryResult);
    });
  }
}

void UpdateAttributesFeatureService::onQueryFeaturesCompleted_(FeatureQueryResult* featureQueryResult)
{
  if (featureQueryResult && featureQueryResult->iterator().hasNext())
  {
    // first delete if not nullptr
    if (m_selectedFeature)
      delete m_selectedFeature;

    // set selected feature member
    m_selectedFeature = static_cast<ArcGISFeature*>(featureQueryResult->iterator().next(this));
    m_selectedFeature->setParent(this);
    m_featureType = m_selectedFeature->attributes()->attributeValue("typdamage").toString();
    m_mapView->calloutData()->setTitle(QString("<br><font size=\"+2\"><b>%1</b></font>").arg(m_featureType));
    m_mapView->calloutData()->setLocation(m_selectedFeature->geometry().extent().center());
    emit featureTypeChanged();
    emit featureSelected();
  }
}

void UpdateAttributesFeatureService::onApplyEditsCompleted_(const QList<FeatureEditResult*>& featureEditResults)
{
  // Lock is a convenience wrapper that deletes the contents of featureEditResults when we leave scope.
  FeatureEditListResultLock lock(featureEditResults);

  // check if result list is not empty
  if (!lock.results.isEmpty())
  {
    // obtain the first item in the list
    FeatureEditResult* featureEditResult = lock.results.first();
    // check if there were errors, and if not, log the new object ID
    if (!featureEditResult->isCompletedWithErrors())
      qDebug() << "Successfully updated attribute for Object ID:" << featureEditResult->objectId();
    else
      qDebug() << "Apply edits error.";
  }
  m_featureLayer->clearSelection();
}
