// [WriteFile Name=DisplayClusters, Category=DisplayInformation]
// [Legal]
// Copyright 2023 Esri.
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

#ifndef DISPLAYCLUSTERS_H
#define DISPLAYCLUSTERS_H

// Qt headers
#include <QMouseEvent>
#include <QObject>

namespace Esri::ArcGISRuntime
{
class AggregateGeoElement;
class FeatureLayer;
class Map;
class MapQuickView;
}

Q_MOC_INCLUDE("MapQuickView.h");

class DisplayClusters : public QObject
{
  Q_OBJECT

  Q_PROPERTY(Esri::ArcGISRuntime::MapQuickView* mapView READ mapView WRITE setMapView NOTIFY mapViewChanged)
  Q_PROPERTY(bool taskRunning READ taskRunning NOTIFY taskRunningChanged)
  Q_PROPERTY(QString calloutText READ calloutText NOTIFY calloutTextChanged)

public:
  explicit DisplayClusters(QObject* parent = nullptr);
  ~DisplayClusters() override;

  Q_INVOKABLE void toggleClustering();

  static void init();

signals:
  void mapViewChanged();
  void taskRunningChanged();
  void calloutTextChanged();

private:
  Esri::ArcGISRuntime::MapQuickView* mapView() const;
  void setMapView(Esri::ArcGISRuntime::MapQuickView* mapView);
  QString calloutText() const;

  void onMouseClicked(const QMouseEvent& mouseEvent);

  Esri::ArcGISRuntime::Map* m_map = nullptr;
  Esri::ArcGISRuntime::MapQuickView* m_mapView = nullptr;

  Esri::ArcGISRuntime::FeatureLayer* m_powerPlantsLayer = nullptr;

  bool taskRunning() const;
  bool m_taskRunning = true;
  QString m_calloutText = "";

  QScopedPointer<QObject> m_resultParent;
  Esri::ArcGISRuntime::AggregateGeoElement* m_aggregateGeoElement = nullptr;
};

#endif // DISPLAYCLUSTERS_H
