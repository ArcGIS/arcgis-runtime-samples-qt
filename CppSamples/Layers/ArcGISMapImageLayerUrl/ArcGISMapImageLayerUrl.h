// [WriteFile Name=ArcGISMapImageLayerUrl, Category=Layers]
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

#ifndef ARCGIS_MAP_IMAGE_LAYER_URL_H
#define ARCGIS_MAP_IMAGE_LAYER_URL_H

// Qt headers
#include <QQuickItem>

namespace Esri::ArcGISRuntime
{
  class Map;
  class MapQuickView;
}

class ArcGISMapImageLayerUrl : public QQuickItem
{
  Q_OBJECT

public:
  explicit ArcGISMapImageLayerUrl(QQuickItem* parent = nullptr);
  ~ArcGISMapImageLayerUrl() override;

  void componentComplete() override;
  static void init();

private:
  Esri::ArcGISRuntime::Map* m_map = nullptr;
  Esri::ArcGISRuntime::MapQuickView* m_mapView = nullptr;
};

#endif // ARCGIS_MAP_IMAGE_LAYER_URL_H
