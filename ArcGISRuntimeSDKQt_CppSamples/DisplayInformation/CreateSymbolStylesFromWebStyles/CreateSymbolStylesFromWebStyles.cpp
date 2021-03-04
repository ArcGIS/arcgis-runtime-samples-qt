// [WriteFile Name=CreateSymbolStylesFromWebStyles, Category=DisplayInformation]
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

#ifdef PCH_BUILD
#include "pch.hpp"
#endif // PCH_BUILD

#include "CreateSymbolStylesFromWebStyles.h"

#include "FeatureLayer.h"
#include "ServiceFeatureTable.h"
#include "Map.h"
#include "MapQuickView.h"

#include "UniqueValueRenderer.h"
#include "SymbolStyle.h"
#include "SimpleMarkerSymbol.h"

using namespace Esri::ArcGISRuntime;

CreateSymbolStylesFromWebStyles::CreateSymbolStylesFromWebStyles(QObject* parent /* = nullptr */):
  QObject(parent),
  m_map(new Map(BasemapStyle::ArcGISNavigation, this))
{
  m_map->setReferenceScale(100'000);

  QUrl webStyleLayerUrl = QUrl("http://services.arcgis.com/V6ZHFr6zdgNZuVG0/arcgis/rest/services/LA_County_Points_of_Interest/FeatureServer/0");
  m_webStyleLayer = new FeatureLayer(new ServiceFeatureTable(webStyleLayerUrl, this), this);
  m_map->operationalLayers()->append(m_webStyleLayer);

  m_uniqueValueRenderer = new UniqueValueRenderer(this);
  m_uniqueValueRenderer->setFieldNames({"cat2"});
  m_webStyleLayer->setRenderer(m_uniqueValueRenderer);

  m_categoriesMap = createCategoriesMap();
  m_connectionIterations = 0;

  for (const QString &symbolKey : m_categoriesMap.keys())
  {
    SymbolStyle* symbolStyle = new SymbolStyle("Esri2DPointSymbolsStyle", {}, this);
    connect(symbolStyle, &SymbolStyle::fetchSymbolCompleted, this, [this, symbolKey](QUuid /* taskId */, Symbol* symbol)
    {
      for (const QString &category : m_categoriesMap[symbolKey])
      {
        m_uniqueValueRenderer->uniqueValues()->append(new UniqueValue(symbolKey, "", {category}, symbol, this));
      }
      qDebug() << symbolKey;
      m_connectionIterations++;
      if (m_categoriesMap.keys().size() == m_connectionIterations)
        m_map->setAutoFetchLegendInfos(true);
    });
    symbolStyle->fetchSymbol({symbolKey});
  }

  connect(m_map->legendInfos(), &LegendInfoListModel::fetchLegendInfosCompleted, this, [this]()
  {
    qDebug() << "fetch legend infos completed";
    m_legendInfoListModel = m_map->legendInfos();
    qDebug() <<  m_map->legendInfos()->rowCount();
    buildLegend();
    emit legendInfoListModelChanged();
  });
}

CreateSymbolStylesFromWebStyles::~CreateSymbolStylesFromWebStyles() = default;

void CreateSymbolStylesFromWebStyles::init()
{
  // Register the map view for QML
  qmlRegisterType<MapQuickView>("Esri.Samples", 1, 0, "MapView");
  qmlRegisterType<CreateSymbolStylesFromWebStyles>("Esri.Samples", 1, 0, "CreateSymbolStylesFromWebStylesSample");
  qmlRegisterUncreatableType<QAbstractListModel>("Esri.Samples", 1, 0, "AbstractListModel", "AbstractListModel is uncreateable");
}

MapQuickView* CreateSymbolStylesFromWebStyles::mapView() const
{
  return m_mapView;
}

// Set the view (created in QML)
void CreateSymbolStylesFromWebStyles::setMapView(MapQuickView* mapView)
{
  if (!mapView || mapView == m_mapView)
    return;

  m_mapView = mapView;
  m_mapView->setMap(m_map);

  const double x_longitude = -118.44186;
  const double y_latitude = 34.28301;
  const Point centerPt(x_longitude, y_latitude, SpatialReference::wgs84());
  const double scale = 7000;

  m_mapView->setViewpointCenter(centerPt, scale);

  connect(m_mapView, &MapQuickView::mapScaleChanged, this, [this]()
  {
    m_webStyleLayer->setScaleSymbols(m_mapView->mapScale() >= 80'000);
  });

  emit mapViewChanged();
}

QMap<QString,QList<QString>> CreateSymbolStylesFromWebStyles::createCategoriesMap()
{
  QMap<QString,QList<QString>> categories;
  categories["atm"] = {"Banking and Finance"};
  categories["beach"] = {"Beaches and Marinas"};
  categories["campground"] = {"Campgrounds"};
  categories["city-hall"] = {"City Halls", "Government Offices"};
  categories["hospital"] = {"Hospitals and Medical Centers", "Health Screening and Testing", "Health Centers", "Mental Health Centers"};
  categories["library"] = {"Libraries"};
  categories["park"] = {"Parks and Gardens"};
  categories["place-of-worship"] = {"Churches"};
  categories["police-station"] = {"Sheriff and Police Stations"};
  categories["post-office"] = {"DHL Locations", "Federal Express Locations"};
  categories["school"] = {"Public High Schools", "Public Elementary Schools", "Private and Charter Schools"};
  categories["trail"] = {"Trails"};
  return categories;
}

void CreateSymbolStylesFromWebStyles::buildLegend()
{
  m_legendInfoList.append(m_map->legendInfos()->at(0));
  for (int i = 1; i < m_map->legendInfos()->rowCount(); ++i)
  {
    if (m_map->legendInfos()->at(i)->name() != m_map->legendInfos()->at(i-1)->name())
    {
      m_legendInfoList.append(m_map->legendInfos()->at(i));
      qDebug() << m_legendInfoList.last()->name();
    }
  }

  qDebug() << m_legendInfoList.size();
  emit legendInfoListChanged();
}