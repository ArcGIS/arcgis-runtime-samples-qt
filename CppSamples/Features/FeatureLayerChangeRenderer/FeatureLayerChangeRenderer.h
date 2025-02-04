// [WriteFile Name=FeatureLayerChangeRenderer, Category=Features]
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

#ifndef FEATURE_LAYER_CHANGE_RENDERER_H
#define FEATURE_LAYER_CHANGE_RENDERER_H

// Qt headers
#include <QQuickItem>

namespace Esri::ArcGISRuntime
{
  class Map;
  class MapQuickView;
  class FeatureLayer;
}

class FeatureLayerChangeRenderer : public QQuickItem
{
  Q_OBJECT

  Q_PROPERTY(bool layerInitialized READ layerInitialized NOTIFY layerInitializedChanged)

public:
  explicit FeatureLayerChangeRenderer(QQuickItem* parent = nullptr);
  ~FeatureLayerChangeRenderer() override;

  void componentComplete() override;
  static void init();
  Q_INVOKABLE void changeRenderer();

signals:
  void layerInitializedChanged();

private:
  bool layerInitialized() const;

private:
  Esri::ArcGISRuntime::Map* m_map = nullptr;
  Esri::ArcGISRuntime::MapQuickView* m_mapView = nullptr;
  Esri::ArcGISRuntime::FeatureLayer* m_featureLayer = nullptr;
  bool m_initialized = false;
  bool m_stateChanged = false;
};

#endif // FEATURE_LAYER_CHANGE_RENDERER_H
