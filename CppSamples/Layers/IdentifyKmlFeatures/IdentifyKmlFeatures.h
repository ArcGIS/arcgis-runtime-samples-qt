// [WriteFile Name=IdentifyKmlFeatures, Category=Layers]
// [Legal]
// Copyright 2020 Esri.
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

#ifndef IDENTIFYKMLFEATURES_H
#define IDENTIFYKMLFEATURES_H

// ArcGIS Maps SDK headers
#include "Point.h"

// Qt headers
#include <QObject>
#include <QString>

namespace Esri::ArcGISRuntime
{
class CalloutData;
class KmlLayer;
class Map;
class MapQuickView;
}

Q_MOC_INCLUDE("MapQuickView.h")
Q_MOC_INCLUDE("CalloutData.h")

class IdentifyKmlFeatures : public QObject
{
  Q_OBJECT

  Q_PROPERTY(Esri::ArcGISRuntime::MapQuickView* mapView READ mapView WRITE setMapView NOTIFY mapViewChanged)
  Q_PROPERTY(Esri::ArcGISRuntime::CalloutData* calloutData READ calloutData NOTIFY calloutDataChanged)
  Q_PROPERTY(QString calloutText READ calloutText NOTIFY calloutTextChanged)

public:
  explicit IdentifyKmlFeatures(QObject* parent = nullptr);
  ~IdentifyKmlFeatures();

  static void init();

signals:
  void mapViewChanged();
  void calloutDataChanged();
  void calloutTextChanged();

private:
  Esri::ArcGISRuntime::MapQuickView* mapView() const;
  void setMapView(Esri::ArcGISRuntime::MapQuickView* mapView);
  Esri::ArcGISRuntime::CalloutData* calloutData() const;
  QString calloutText() const;

  Esri::ArcGISRuntime::Map* m_map = nullptr;
  Esri::ArcGISRuntime::MapQuickView* m_mapView = nullptr;
  Esri::ArcGISRuntime::CalloutData* m_calloutData = nullptr;
  Esri::ArcGISRuntime::KmlLayer* m_forecastLayer = nullptr;
  Esri::ArcGISRuntime::Point m_clickedPoint;
  QString m_calloutText;
};

#endif // IDENTIFYKMLFEATURES_H
