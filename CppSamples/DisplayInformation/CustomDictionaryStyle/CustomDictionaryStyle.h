// [WriteFile Name=CustomDictionaryStyle, Category=DisplayInformation]
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

#ifndef CUSTOMDICTIONARYSTYLE_H
#define CUSTOMDICTIONARYSTYLE_H

// Qt headers
#include <QObject>

namespace Esri::ArcGISRuntime
{
class DictionaryRenderer;
class FeatureLayer;
class Map;
class MapQuickView;
}

Q_MOC_INCLUDE("MapQuickView.h")

class CustomDictionaryStyle : public QObject
{
  Q_OBJECT

  Q_PROPERTY(Esri::ArcGISRuntime::MapQuickView* mapView READ mapView WRITE setMapView NOTIFY mapViewChanged)

public:
  explicit CustomDictionaryStyle(QObject* parent = nullptr);
  ~CustomDictionaryStyle();

  static void init();

  Q_INVOKABLE void changeDictionarySymbolStyleSource();

signals:
  void mapViewChanged();

private:
  Esri::ArcGISRuntime::MapQuickView* mapView() const;
  void setMapView(Esri::ArcGISRuntime::MapQuickView* mapView);

  Esri::ArcGISRuntime::Map* m_map = nullptr;
  Esri::ArcGISRuntime::MapQuickView* m_mapView = nullptr;
  Esri::ArcGISRuntime::DictionaryRenderer* m_localDictionaryRenderer = nullptr;
  Esri::ArcGISRuntime::DictionaryRenderer* m_webDictionaryRenderer = nullptr;
  Esri::ArcGISRuntime::FeatureLayer* m_featureLayer = nullptr;
};

#endif // CUSTOMDICTIONARYSTYLE_H
