// [WriteFile Name=OfflineGeocode, Category=Search]
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

#ifdef PCH_BUILD
#include "pch.hpp"
#endif // PCH_BUILD

// sample headers
#include "OfflineGeocode.h"

// ArcGIS Maps SDK headers
#include "ArcGISTiledLayer.h"
#include "Basemap.h"
#include "CalloutData.h"
#include "Error.h"
#include "ErrorException.h"
#include "GeocodeParameters.h"
#include "GeocodeResult.h"
#include "Graphic.h"
#include "GraphicListModel.h"
#include "GraphicsOverlay.h"
#include "GraphicsOverlayListModel.h"
#include "IdentifyGraphicsOverlayResult.h"
#include "LayerListModel.h"
#include "LocatorTask.h"
#include "Map.h"
#include "MapQuickView.h"
#include "MapViewTypes.h"
#include "PictureMarkerSymbol.h"
#include "Point.h"
#include "SpatialReference.h"
#include "SuggestListModel.h"
#include "TileCache.h"
#include "Viewpoint.h"

// Qt headers
#include <QFuture>
#include <QScopedPointer>
#include <QStandardPaths>
#include <QUuid>
#include <QtCore/qglobal.h>

// STL headers
#include <memory>

using namespace Esri::ArcGISRuntime;

// helper method to get cross platform data path
namespace
{
  QString defaultDataPath()
  {
    QString dataPath;

  #ifdef Q_OS_IOS
    dataPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
  #else
    dataPath = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
  #endif

    return dataPath;
  }
} // namespace

using namespace Esri::ArcGISRuntime;

OfflineGeocode::OfflineGeocode(QQuickItem* parent):
  QQuickItem(parent),
  m_dataPath(defaultDataPath() + "/ArcGIS/Runtime/Data/")
{
}

OfflineGeocode::~OfflineGeocode() = default;

void OfflineGeocode::init()
{
  qmlRegisterType<MapQuickView>("Esri.Samples", 1, 0, "MapView");
  qmlRegisterType<OfflineGeocode>("Esri.Samples", 1, 0, "OfflineGeocodeSample");
  qmlRegisterUncreatableType<CalloutData>("Esri.Samples", 1, 0, "CalloutData", "CalloutData is an uncreatable type");
  qmlRegisterUncreatableType<SuggestListModel>("Esri.Samples", 1, 0, "SuggestListModel", "SuggestionListModel is an uncreatable type");
}

void OfflineGeocode::componentComplete()
{
  QQuickItem::componentComplete();

  // find QML MapView component
  m_mapView = findChild<MapQuickView*>("mapView");
  m_mapView->setWrapAroundMode(WrapAroundMode::Disabled);

  // create a tiled layer using a local .tpkx file
  TileCache* tileCache = new TileCache(m_dataPath + "tpkx/streetmap_SD.tpkx", this);
  connect(tileCache, &TileCache::errorOccurred, this, &OfflineGeocode::logError);

  m_tiledLayer = new ArcGISTiledLayer(tileCache, this);

  // create basemap and add tiled layer
  Basemap* basemap = new Basemap(this);
  basemap->baseLayers()->append(m_tiledLayer);

  // create map using basemap
  m_map = new Map(basemap, this);
  m_map->setInitialViewpoint(Viewpoint(Point(-13042254.715252, 3857970.236806, SpatialReference(3857)), 2e4));

  // Set map to map view
  m_mapView->setMap(m_map);

  // create locator task
  //! [OfflineGeocode create LocatorTask]
  m_locatorTask = new LocatorTask(m_dataPath + "Locators/SanDiegoStreetAddressLocator/SanDiego_StreetAddress.loc", this);
  //! [OfflineGeocode create LocatorTask]

  // set the suggestions Q_PROPERTY
  m_suggestListModel = m_locatorTask->suggestions();
  emit suggestionsChanged();

  // set geocode parameters
  m_geocodeParameters.setMinScore(75);
  m_geocodeParameters.setMaxResults(1);
  m_reverseGeocodeParameters.setMaxResults(1);

  // create graphics overlay and add pin graphic
  m_graphicsOverlay = new GraphicsOverlay(this);
  PictureMarkerSymbol* pinSymbol = new PictureMarkerSymbol(QUrl("qrc:/Samples/Search/OfflineGeocode/red_pin.png"), this);
  pinSymbol->setHeight(36);
  pinSymbol->setWidth(19);
  pinSymbol->setOffsetY(pinSymbol->height() / 2);
  m_pinGraphic = new Graphic(Point(-13042254.715252, 3857970.236806, SpatialReference(3857)), pinSymbol, this);
  m_pinGraphic->setVisible(false);
  m_graphicsOverlay->graphics()->append(m_pinGraphic);
  m_mapView->graphicsOverlays()->append(m_graphicsOverlay);

  // initialize callout
  m_mapView->calloutData()->setVisible(false);
  m_mapView->calloutData()->setTitle("Address");
  m_calloutData = m_mapView->calloutData();

  connectSignals();
}

void OfflineGeocode::geocodeWithText(const QString& address)
{
  m_locatorTask->geocodeWithParametersAsync(address, m_geocodeParameters).then(this, [this](const QList<GeocodeResult>& geocodeResults)
  {
    onGeocodingCompleted_(geocodeResults);
  }).onFailed(this, [this](const ErrorException& e)
  {
    logException(e);
  });
}

void OfflineGeocode::geocodeWithSuggestion(int index)
{
  m_locatorTask->geocodeWithSuggestResultAndParametersAsync(m_suggestListModel->suggestResults().at(index), m_geocodeParameters)
      .then(this, [this](const QList<GeocodeResult>& geocodeResults)
  {
    onGeocodingCompleted_(geocodeResults);
  }).onFailed(this, [this](const ErrorException& e)
  {
    logException(e);
  });
}

void OfflineGeocode::setSuggestionsText(const QString& searchText)
{
  m_suggestListModel->setSearchText(searchText);
}

void OfflineGeocode::logError(const Error& error)
{
  setErrorMessage( QString("%1: %2").arg(error.message(), error.additionalMessage()));
}

void OfflineGeocode::logException(const ErrorException& exception)
{
  logError(exception.error());
}

SuggestListModel* OfflineGeocode::suggestions() const
{
  return m_suggestListModel;
}

bool OfflineGeocode::geocodeInProgress() const
{
  return m_geocodeInProgress;
}

bool OfflineGeocode::noResults() const
{
  return m_noResults;
}

bool OfflineGeocode::suggestInProgress() const
{
  return m_suggestInProgress;
}

void OfflineGeocode::connectSignals()
{
  connect(m_map, &Map::errorOccurred, this, &OfflineGeocode::logError);
  connect(m_mapView, &MapQuickView::mouseClicked, this, [this](QMouseEvent& mouseEvent)
  {
    m_clickedPoint = m_mapView->screenToLocation(mouseEvent.position().x(), mouseEvent.position().y());
    m_mapView->identifyGraphicsOverlayAsync(m_graphicsOverlay, mouseEvent.position(), 5, false, 1)
        .then(this, [this](IdentifyGraphicsOverlayResult* rawIdentifyResult)
    {
      // Delete rawIdentifyResult when we leave scope.
      auto identifyResult = std::unique_ptr<IdentifyGraphicsOverlayResult>(rawIdentifyResult);

      if (!identifyResult)
        return;

      // if user clicked on pin, display callout
      const QList<Graphic*> graphics = identifyResult->graphics();
      if (graphics.count() > 0)
        m_calloutData->setVisible(true);

      // otherwise, reverse geocode at that point
      else
      {
        m_isReverseGeocode = true;
        m_locatorTask->reverseGeocodeWithParametersAsync(m_clickedPoint, m_reverseGeocodeParameters)
            .then(this, [this](const QList<GeocodeResult>& geocodeResults)
        {
          onGeocodingCompleted_(geocodeResults);
        }).onFailed(this, [this](const ErrorException& e)
        {
          logException(e);
        });

        m_geocodeInProgress = true;
        emit geocodeInProgressChanged();
      }
    }).onFailed(this, [this](const ErrorException& e)
    {
      logException(e);
    });
  });

  connect(m_mapView, &MapQuickView::mousePressedAndHeld, this, [this](QMouseEvent& mouseEvent)
  {
    m_isPressAndHold = true;
    m_isReverseGeocode = true;

    // reverse geocode
    m_locatorTask->reverseGeocodeWithParametersAsync(Point(m_mapView->screenToLocation(mouseEvent.position().x(), mouseEvent.position().y())), m_reverseGeocodeParameters)
        .then(this, [this](const QList<GeocodeResult>& geocodeResults)
    {
      onGeocodingCompleted_(geocodeResults);
    }).onFailed(this, [this](const ErrorException& e)
    {
      logException(e);
    });

    // make busy indicator visible
    m_geocodeInProgress = true;
    emit geocodeInProgressChanged();
  });

  connect(m_mapView, &MapQuickView::mouseMoved, this, [this](QMouseEvent& mouseEvent)
  {
    // if user is dragging mouse hold, realtime reverse geocode
    if (m_isPressAndHold)
    {
      m_locatorTask->reverseGeocodeWithParametersAsync(Point(m_mapView->screenToLocation(mouseEvent.position().x(), mouseEvent.position().y())), m_reverseGeocodeParameters)
          .then(this, [this](const QList<GeocodeResult>& geocodeResults)
      {
        onGeocodingCompleted_(geocodeResults);
      }).onFailed(this, [this](const ErrorException& e)
      {
        logException(e);
      });

      m_geocodeInProgress = true;
      emit geocodeInProgressChanged();
    }
  });

  // reset after user stops holding down mouse
  connect(m_mapView, &MapQuickView::mouseReleased, this, [this]()
  {
    m_isPressAndHold = false;
    m_isReverseGeocode = false;
  });

  // dismiss no results notification and suggestions on mouse press
  connect(m_mapView, &MapQuickView::mousePressed, this, [this]()
  {
    m_noResults = false;
    emit noResultsChanged();
    emit dismissSuggestions();
  });

  connect(m_suggestListModel, &SuggestListModel::errorOccurred, this, &OfflineGeocode::logError);
  connect(m_suggestListModel, &SuggestListModel::suggestInProgressChanged, this, [this]()
  {
    m_suggestInProgress = m_suggestListModel->suggestInProgress();
    emit suggestInProgressChanged();
  });
}

void OfflineGeocode::onGeocodingCompleted_(const QList<GeocodeResult>& geocodeResults)
{
  // dismiss busy indicator
  m_geocodeInProgress = false;
  emit geocodeInProgressChanged();

  // dismiss callouts
  m_calloutData->setVisible(false);
  m_pinGraphic->setVisible(false);

  // if there are no matching results, notify user and stop processing
  if (geocodeResults.isEmpty())
  {
    m_noResults = true;
    emit noResultsChanged();
    return;
  }

  // dismiss no results notification
  m_noResults = false;
  emit noResultsChanged();

  // zoom to result's extent
  m_mapView->setViewpointCenterAsync(geocodeResults.at(0).displayLocation());

  // set pin graphic's location
  m_pinGraphic->setGeometry(geocodeResults.at(0).displayLocation());
  m_pinGraphic->setVisible(true);

  // set callout location and detail
  m_calloutData->setDetail(geocodeResults.at(0).label());
  m_calloutData->setGeoElement(m_pinGraphic);

  if (m_isReverseGeocode)
    m_calloutData->setVisible(true);

  // continue reverse geocoding if user is pressing and holding
  if (!m_isPressAndHold)
    m_isReverseGeocode = false;
}

QString OfflineGeocode::errorMessage() const
{
  return m_errorMsg;
}

void OfflineGeocode::setErrorMessage(const QString& msg)
{
  m_errorMsg = msg;
  qDebug() << m_errorMsg;
  emit errorMessageChanged();
}
