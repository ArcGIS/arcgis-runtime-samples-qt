// [WriteFile Name=AddGraphicsWithRenderer, Category=DisplayInformation]
// [Legal]
// Copyright 2022 Esri.

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

#ifndef ADDGRAPHICSWITHRENDERER_H
#define ADDGRAPHICSWITHRENDERER_H

namespace Esri::ArcGISRuntime
{
class Geometry;
class Graphic;
class Map;
class MapQuickView;
class Symbol;
}

#include <QObject>

Q_MOC_INCLUDE("MapQuickView.h");

class AddGraphicsWithRenderer : public QObject
{
  Q_OBJECT

  Q_PROPERTY(Esri::ArcGISRuntime::MapQuickView* mapView READ mapView WRITE setMapView NOTIFY mapViewChanged)

public:
  explicit AddGraphicsWithRenderer(QObject* parent = nullptr);
  ~AddGraphicsWithRenderer() override;

  static void init();

signals:
  void mapViewChanged();

private:
  Esri::ArcGISRuntime::MapQuickView* mapView() const;
  void setMapView(Esri::ArcGISRuntime::MapQuickView* mapView);

  void addCurveGraphic();
  void addEllipseGraphic();
  void addGraphicsOverlays();
  void addLineGraphic();
  void addPointGraphic();
  void addPolygonGraphic();
  void createGraphicsOverlayWithGraphicAndSymbol(Esri::ArcGISRuntime::Graphic* graphic, Esri::ArcGISRuntime::Symbol* symbol);
  Esri::ArcGISRuntime::Geometry createHeart();

  Esri::ArcGISRuntime::Map* m_map = nullptr;
  Esri::ArcGISRuntime::MapQuickView* m_mapView = nullptr;
};

#endif // ADDGRAPHICSWITHRENDERER_H
