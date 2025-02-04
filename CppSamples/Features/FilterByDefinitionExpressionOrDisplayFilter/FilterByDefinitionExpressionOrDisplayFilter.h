// [WriteFile Name=FilterByDefinitionExpressionOrDisplayFilter, Category=Features]
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

#ifndef FILTER_BY_DEFINITION_EXPRESSION_OR_DISPLAY_FILTER_H
#define FILTER_BY_DEFINITION_EXPRESSION_OR_DISPLAY_FILTER_H

// Qt headers
#include <QQuickItem>

namespace Esri::ArcGISRuntime
{
  class Map;
  class MapQuickView;
  class FeatureLayer;
  class ServiceFeatureTable;
}

class FilterByDefinitionExpressionOrDisplayFilter : public QQuickItem
{
  Q_OBJECT

  Q_PROPERTY(bool layerInitialized READ layerInitialized NOTIFY layerInitializedChanged)
  Q_PROPERTY(int currentFeatureCount READ currentFeatureCount NOTIFY currentFeatureCountChanged)
  Q_PROPERTY(bool mapDrawing READ mapDrawing NOTIFY mapDrawStatusChanged)

public:
  explicit FilterByDefinitionExpressionOrDisplayFilter(QQuickItem* parent = nullptr);
  ~FilterByDefinitionExpressionOrDisplayFilter() override;

  void componentComplete() override;
  static void init();
  Q_INVOKABLE void setDefExpression(const QString& whereClause);
  Q_INVOKABLE void setDisplayFilter(const QString& whereClause);
  Q_INVOKABLE void resetDisplayFilterParams();
  Q_INVOKABLE void resetDefExpressionParams();

signals:
  void layerInitializedChanged();
  void currentFeatureCountChanged();
  void mapDrawStatusChanged();

private:
  bool layerInitialized() const;
  int currentFeatureCount() const;
  void queryFeatureCountInCurrentExtent();
  bool mapDrawing() const;

private:
  Esri::ArcGISRuntime::Map* m_map = nullptr;
  Esri::ArcGISRuntime::MapQuickView* m_mapView = nullptr;
  Esri::ArcGISRuntime::FeatureLayer* m_featureLayer = nullptr;
  bool m_initialized = false;
  Esri::ArcGISRuntime::ServiceFeatureTable* m_featureTable = nullptr;
  int m_currentFeatureCount = 0;
  bool m_mapDrawing = false;
};

#endif // FILTER_BY_DEFINITION_EXPRESSION_OR_DISPLAY_FILTER_H
