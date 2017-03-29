// [WriteFile Name=DeleteFeaturesFeatureService, Category=EditData]
// [Legal]
// Copyright 2016 Esri.

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

#ifndef DELETE_FEATURES_FEATURE_SERVICE_H
#define DELETE_FEATURES_FEATURE_SERVICE_H

namespace Esri
{
  namespace ArcGISRuntime
  {
    class Map;
    class MapQuickView;
    class FeatureLayer;
    class ServiceFeatureTable;
    class Feature;
  }
}

class QString;

#include <QQuickItem>

class DeleteFeaturesFeatureService : public QQuickItem
{
  Q_OBJECT

  Q_PROPERTY(int screenX READ screenX NOTIFY screenXChanged)
  Q_PROPERTY(int screenY READ screenY NOTIFY screenYChanged)
  Q_PROPERTY(QString featureType READ featureType NOTIFY featureTypeChanged)

public:
  explicit DeleteFeaturesFeatureService(QQuickItem* parent = nullptr);
  ~DeleteFeaturesFeatureService();

  void componentComplete() Q_DECL_OVERRIDE;
  static void init();
  Q_INVOKABLE void deleteSelectedFeature();

signals:
  void screenXChanged();
  void screenYChanged();
  void featureSelected();
  void featureTypeChanged();
  void hideWindow();

private:
  void connectSignals();
  int screenX() const;
  int screenY() const;
  QString featureType() const;

private:
  Esri::ArcGISRuntime::Map* m_map = nullptr;
  Esri::ArcGISRuntime::MapQuickView* m_mapView = nullptr;
  Esri::ArcGISRuntime::FeatureLayer* m_featureLayer = nullptr;
  Esri::ArcGISRuntime::ServiceFeatureTable* m_featureTable = nullptr;
  Esri::ArcGISRuntime::Feature* m_selectedFeature = nullptr;
  int m_screenX = 0;
  int m_screenY = 0;
  QString m_featureType;
};

#endif // DELETE_FEATURES_FEATURE_SERVICE_H

