// [WriteFile Name=OfflineRouting, Category=Analysis]
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

#ifndef OFFLINEROUTING_H
#define OFFLINEROUTING_H

namespace Esri
{
namespace ArcGISRuntime
{
class Graphic;
class GraphicsOverlay;
class Map;
class MapQuickView;
class RouteParameters;
class RouteTask;
}
}

//class CustomMapView;

#include <QObject>

#include "Point.h"
#include "RouteParameters.h"

class OfflineRouting : public QObject
{
  Q_OBJECT

  Q_PROPERTY(Esri::ArcGISRuntime::MapQuickView* mapView READ mapView WRITE setMapView NOTIFY mapViewChanged)
  Q_PROPERTY(QStringList travelModeNames READ travelModeNames NOTIFY travelModeNamesChanged)
  Q_PROPERTY(int travelModeIndex READ travelModeIndex WRITE setTravelModeIndex NOTIFY travelModeIndexChanged)

public:
  explicit OfflineRouting(QObject* parent = nullptr);
  ~OfflineRouting();

  static void init();

  Q_INVOKABLE void findRoute();
//  Q_INVOKABLE int getTravelMode();

  bool eventFilter(QObject* obj, QEvent* event) override;
  void connectSignals();

signals:
  void mapViewChanged();
  void travelModeNamesChanged();
  void travelModeIndexChanged();

private:
  Esri::ArcGISRuntime::MapQuickView* mapView() const;
  void setMapView(Esri::ArcGISRuntime::MapQuickView* mapView);

  QStringList travelModeNames();
  int travelModeIndex();
  void setTravelModeIndex(int index);

  Esri::ArcGISRuntime::Map* m_map = nullptr;
  Esri::ArcGISRuntime::MapQuickView* m_mapView = nullptr;

  Esri::ArcGISRuntime::Graphic* m_selectedGraphic = nullptr;
  Esri::ArcGISRuntime::GraphicsOverlay* m_stopsOverlay = nullptr;
  Esri::ArcGISRuntime::GraphicsOverlay* m_routeOverlay = nullptr;
  Esri::ArcGISRuntime::RouteTask* m_routeTask = nullptr;
  Esri::ArcGISRuntime::RouteParameters m_routeParameters;
  Esri::ArcGISRuntime::Point m_clickedPoint;
  int m_travelModeIndex = 0;
  bool m_selectedStop = false;
};

#endif // OFFLINEROUTING_H
