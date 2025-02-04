// [WriteFile Name=CutGeometry, Category=Geometry]
// [Legal]
// Copyright 2018 Esri.
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

#ifndef CUTGEOMETRY_H
#define CUTGEOMETRY_H

// ArcGIS Maps SDK headers
#include "Polygon.h"
#include "Polyline.h"

// Qt headers
#include <QQuickItem>

namespace Esri::ArcGISRuntime
{
class Map;
class MapQuickView;
class Graphic;
class GraphicsOverlay;
}

class CutGeometry : public QQuickItem
{
  Q_OBJECT

public:
  explicit CutGeometry(QQuickItem* parent = nullptr);
  ~CutGeometry() override = default;

  void componentComplete() override;
  static void init();
  Q_INVOKABLE void cutPolygon();
  Q_INVOKABLE void resetPolygon();

private:
  Esri::ArcGISRuntime::Map* m_map = nullptr;
  Esri::ArcGISRuntime::MapQuickView* m_mapView = nullptr;
  Esri::ArcGISRuntime::Graphic* m_lakeSuperiorGraphic = nullptr;
  Esri::ArcGISRuntime::Graphic* m_borderGraphic = nullptr;
  Esri::ArcGISRuntime::GraphicsOverlay* m_overlay = nullptr;

private:
  void createGraphics();
  Esri::ArcGISRuntime::Polygon createLakeSuperiorPolygon();
  Esri::ArcGISRuntime::Polyline createBorderPolyline();
};

#endif // CUTGEOMETRY_H
