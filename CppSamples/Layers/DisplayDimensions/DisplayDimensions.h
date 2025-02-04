// [WriteFile Name=DisplayDimensions, Category=Layers]
// [Legal]
// Copyright 2021 Esri.
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

#ifndef DISPLAYDIMENSIONS_H
#define DISPLAYDIMENSIONS_H

// Qt headers
#include <QObject>

namespace Esri::ArcGISRuntime
{
class DimensionLayer;
class Error;
class Map;
class MapQuickView;
class MobileMapPackage;
}

Q_MOC_INCLUDE("MapQuickView.h")

class DisplayDimensions : public QObject
{
  Q_OBJECT

  Q_PROPERTY(Esri::ArcGISRuntime::MapQuickView* mapView READ mapView WRITE setMapView NOTIFY mapViewChanged)
  Q_PROPERTY(QString errorMessage READ errorMessage WRITE setErrorMessage NOTIFY errorMessageChanged)
  Q_PROPERTY(QString dimensionLayerName READ dimensionLayerName WRITE setDimensionLayerName NOTIFY dimensionLayerNameChanged)
  Q_PROPERTY(bool dimensionLayerVisible READ dimensionLayerVisible WRITE setDimensionLayerVisible)
  Q_PROPERTY(bool useDefinitionExpression READ useDefinitionExpression WRITE setUseDefinitionExpression)
  Q_PROPERTY(bool dimensionsAvailable READ dimensionsAvailable NOTIFY dimensionsAvailableChanged)

public:
  explicit DisplayDimensions(QObject* parent = nullptr);
  ~DisplayDimensions();

  static void init();

signals:
  void mapViewChanged();
  void errorMessageChanged();
  void dimensionLayerNameChanged();
  void dimensionsAvailableChanged();

private:
  Esri::ArcGISRuntime::MapQuickView* mapView() const;
  void setMapView(Esri::ArcGISRuntime::MapQuickView* mapView);
  QString errorMessage();
  void setErrorMessage(const QString& message);
  QString dimensionLayerName() const;
  void setDimensionLayerName(const QString& name);
  void setDimensionLayerVisible(bool visible);
  bool dimensionLayerVisible() const;
  bool useDefinitionExpression() const;
  void setUseDefinitionExpression(bool applied);
  bool dimensionsAvailable();
  void setDimensionsAvailable(bool status);

  void addMapToMapView(const Esri::ArcGISRuntime::Error& error);
  void handleError(const Esri::ArcGISRuntime::Error& error);
  void findDimensionLayer();

  Esri::ArcGISRuntime::Map* m_map = nullptr;
  Esri::ArcGISRuntime::MapQuickView* m_mapView = nullptr;
  Esri::ArcGISRuntime::MobileMapPackage* m_mmpk = nullptr;
  QString m_errorMessage;
  Esri::ArcGISRuntime::DimensionLayer* m_dimensionLayer = nullptr;
  QString m_dimensionLayerName;
  bool m_dimensionsAvailable;
};

#endif // DISPLAYDIMENSIONS_H
