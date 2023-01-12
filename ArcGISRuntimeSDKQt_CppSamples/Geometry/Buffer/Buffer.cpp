
// TRADE SECRETS: ESRI PROPRIETARY AND CONFIDENTIAL
// Unpublished material - all rights reserved under the
// Copyright Laws of the United States and applicable international
// laws, treaties, and conventions.
//
// For additional information, contact:
// Environmental Systems Research Institute, Inc.
// Attn: Contracts and Legal Services Department
// 380 New York Street
// Redlands, California, 92373
// USA
//
// email: contracts@esri.com
/// \file Buffer.cpp

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

#ifdef PCH_BUILD
#include "pch.hpp"
#endif // PCH_BUILD

#include "Buffer.h"

#include "GeometryEngine.h"
#include "Graphic.h"
#include "GraphicListModel.h"
#include "GraphicsOverlay.h"
#include "GraphicsOverlayListModel.h"
#include "LinearUnit.h"
#include "Map.h"
#include "MapQuickView.h"
#include "MapTypes.h"
#include "Point.h"
#include "Polygon.h"
#include "SimpleFillSymbol.h"
#include "SimpleLineSymbol.h"
#include "SimpleMarkerSymbol.h"
#include "SimpleRenderer.h"
#include "SymbolTypes.h"

#include <cmath>

using namespace Esri::ArcGISRuntime;

Buffer::Buffer(QObject* parent /* = nullptr */):
  QObject(parent),
  m_map(new Map(BasemapStyle::ArcGISTopographic, this))
{
}

Buffer::~Buffer() = default;

void Buffer::init()
{
  // Register the map view for QML
  qmlRegisterType<MapQuickView>("Esri.Samples", 1, 0, "MapView");
  qmlRegisterType<Buffer>("Esri.Samples", 1, 0, "BufferSample");
}

MapQuickView* Buffer::mapView() const
{
  return m_mapView;
}

// Set the view (created in QML)
void Buffer::setMapView(MapQuickView* mapView)
{
  if (!mapView || mapView == m_mapView)
    return;

  m_mapView = mapView;
  m_mapView->setMap(m_map);

  // add a graphics overlay for the geodesic buffer
  m_graphicsOverlayGeodesic = new GraphicsOverlay(this);
  m_mapView->graphicsOverlays()->append(m_graphicsOverlayGeodesic);

  // add a graphics overlay for the planar buffer
  m_graphicsOverlayPlanar = new GraphicsOverlay(this);
  m_mapView->graphicsOverlays()->append(m_graphicsOverlayPlanar);

  // add a graphics overlay for the mouse click
  m_graphicsOverlayPoints = new GraphicsOverlay(this);
  m_mapView->graphicsOverlays()->append(m_graphicsOverlayPoints);

  // create symbol for the geodesic buffer result
  SimpleLineSymbol* outlineGeodesic = new SimpleLineSymbol(SimpleLineSymbolStyle::Solid, QColor("red"), 2.0f /*width*/, this);
  SimpleFillSymbol* sfsGeodesic = new SimpleFillSymbol(SimpleFillSymbolStyle::Solid, QColor(255, 0, 0, 122), outlineGeodesic, this);
  m_graphicsOverlayGeodesic->setRenderer(new SimpleRenderer(sfsGeodesic, this));

  // create symbol for the planar buffer result
  SimpleLineSymbol* outlinePlanar = new SimpleLineSymbol(SimpleLineSymbolStyle::Solid, QColor("blue"), 2.0f /*width*/, this);
  SimpleFillSymbol* sfsPlanar = new SimpleFillSymbol(SimpleFillSymbolStyle::Solid, QColor(0, 0, 255, 122), outlinePlanar, this);
  m_graphicsOverlayPlanar->setRenderer(new SimpleRenderer(sfsPlanar, this));

  // create symbol for the mouse click
  SimpleMarkerSymbol* sms = new SimpleMarkerSymbol(SimpleMarkerSymbolStyle::Cross, QColor("white"), 14.0f /*width*/, this);
  m_graphicsOverlayPoints->setRenderer(new SimpleRenderer(sms, this));

  // connect to mouse clicked signal
  connect(m_mapView, &MapQuickView::mouseClicked, this, &Buffer::onMouseClicked);

  emit mapViewChanged();
}

// handle the mouse click - perform a buffer on click
void Buffer::onMouseClicked(QMouseEvent& mouse)
{
  // get the map point from the mouse click
  const Point point = m_mapView->screenToLocation(mouse.position().x(), mouse.position().y());

  // Create a variable to be the buffer size in meters. There are 1609.34 meters in one mile.
  const int bufferInMeters = bufferSize() * 1609.34;

  // Create a planar buffer graphic around the input location at the specified distance.
  const Geometry buffer = GeometryEngine::buffer(point, bufferInMeters);

  // Add the result planar buffer as a graphic
  Graphic* resultGraphicPlanar = new Graphic(buffer, this);
  m_graphicsOverlayPlanar->graphics()->append(resultGraphicPlanar);

  // Create a geodesic buffer graphic around the input location at the specified distance.
  const Geometry bufferGeodesic = GeometryEngine::bufferGeodetic(point, bufferInMeters, LinearUnit(LinearUnitId::Meters), NAN, GeodeticCurveType::Geodesic);

  // Add the result geodesic buffer as a graphic
  Graphic* resultGraphicGeodesic = new Graphic(bufferGeodesic, this);
  m_graphicsOverlayGeodesic->graphics()->append(resultGraphicGeodesic);

  // Add the clicked point as a graphic
  Graphic* clickedPointGraphic = new Graphic(point, this);
  m_graphicsOverlayPoints->graphics()->append(clickedPointGraphic);
}

int Buffer::bufferSize() const
{
  return m_bufferSize;
}

void Buffer::setBufferSize(int size)
{
  if (size == m_bufferSize)
    return;

  m_bufferSize = size;
}

void Buffer::clear()
{
  m_graphicsOverlayGeodesic->graphics()->clear();
  m_graphicsOverlayPlanar->graphics()->clear();
  m_graphicsOverlayPoints->graphics()->clear();
}
