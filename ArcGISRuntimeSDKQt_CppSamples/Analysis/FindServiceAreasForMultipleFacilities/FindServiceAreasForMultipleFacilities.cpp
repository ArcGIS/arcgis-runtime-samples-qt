// [WriteFile Name=FindServiceAreasForMultipleFacilities, Category=Analysis]
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

#include "FindServiceAreasForMultipleFacilities.h"

#include "GraphicsOverlay.h"
#include "Map.h"
#include "MapQuickView.h"

using namespace Esri::ArcGISRuntime;

FindServiceAreasForMultipleFacilities::FindServiceAreasForMultipleFacilities(QObject* parent /* = nullptr */):
  QObject(parent),
  m_map(new Map(Basemap::lightGrayCanvas(this), this))
{
  m_serviceAreasOverlay = new GraphicsOverlay(this);
}

FindServiceAreasForMultipleFacilities::~FindServiceAreasForMultipleFacilities() = default;

void FindServiceAreasForMultipleFacilities::init()
{
  // Register the map view for QML
  qmlRegisterType<MapQuickView>("Esri.Samples", 1, 0, "MapView");
  qmlRegisterType<FindServiceAreasForMultipleFacilities>("Esri.Samples", 1, 0, "FindServiceAreasForMultipleFacilitiesSample");
}

MapQuickView* FindServiceAreasForMultipleFacilities::mapView() const
{
  return m_mapView;
}

// Set the view (created in QML)
void FindServiceAreasForMultipleFacilities::setMapView(MapQuickView* mapView)
{
  if (!mapView || mapView == m_mapView)
    return;

  m_mapView = mapView;
  m_mapView->setMap(m_map);

  m_mapView->graphicsOverlays()->append(m_serviceAreasOverlay);

  emit mapViewChanged();
}
