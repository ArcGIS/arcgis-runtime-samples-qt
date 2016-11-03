// Copyright 2016 ESRI
//
// All rights reserved under the copyright laws of the United States
// and applicable international laws, treaties, and conventions.
//
// You may freely redistribute and use this sample code, with or
// without modification, provided you include the original copyright
// notice and use restrictions.
//
// See the Sample code usage restrictions document for further information.
//

#include "AuthenticationManager.h"
#include "Map.h"
#include "MapQuickView.h"
#include "Basemap.h"
#include "BasemapListModel.h"
#include "Portal.h"
#include "PortalItem.h"
#include "PortalItemListModel.h"
#include "PortalQueryParametersForItems.h"

#include "ShowOrgBasemapPicker.h"

using namespace Esri::ArcGISRuntime;

ShowOrgBasemapPicker::ShowOrgBasemapPicker(QQuickItem* parent /* = nullptr */):
    QQuickItem(parent),
    m_map(nullptr),
    m_mapView(nullptr),
    m_portal(new Portal(new Credential(OAuthClientInfo("W3hPKzPbeJ0tr8aj", OAuthMode::User), this), this))
{
    connect(m_portal, &Portal::basemapsChanged, this, &ShowOrgBasemapPicker::onFetchBasemapsCompleted);
    emit authManagerChanged();
    AuthenticationManager::instance()->setCredentialCacheEnabled(false);
}

ShowOrgBasemapPicker::~ShowOrgBasemapPicker()
{
}

void ShowOrgBasemapPicker::componentComplete()
{
    QQuickItem::componentComplete();

    connect(m_portal, &Portal::loadStatusChanged, this, [this]()
        {
            m_portalLoaded = m_portal->loadStatus() == LoadStatus::Loaded;
            emit portalLoadedChanged();
            emit orgNameChanged();

            if (m_portalLoaded)
                m_portal->fetchBasemaps();
        }
    );
    m_portal->load();

    // find QML MapView component
    m_mapView = findChild<MapQuickView*>("mapView");
    m_mapView->setWrapAroundMode(WrapAroundMode::Disabled);
}

bool ShowOrgBasemapPicker::portalLoaded() const
{
    return m_portalLoaded;
}

QString ShowOrgBasemapPicker::orgName() const
{
    if (!portalLoaded() || !m_portal->portalInfo())
        return "";

    return m_portal->portalInfo()->organizationName();
}

QAbstractListModel* ShowOrgBasemapPicker::basemaps() const
{
    return m_portal->basemaps();
}

QString ShowOrgBasemapPicker::mapLoadError() const
{
    return m_mapLoadeError;
}

void ShowOrgBasemapPicker::loadSelectedBasemap(int index)
{
    if (!m_portal->basemaps())
        return;

    Basemap* selectedBasemap = m_portal->basemaps()->at(index);
    if (!selectedBasemap)
        return;

    if (m_map)
        m_map->disconnect();

    m_mapLoadeError.clear();
    emit mapLoadErrorChanged();

    m_map = new Map(selectedBasemap, this);

    connect(m_map, &Map::errorOccurred, this, [this]()
        {
            m_mapLoadeError = m_map->loadError().message();
            emit mapLoadErrorChanged();
        }
    );

    connect(m_map, &Map::loadStatusChanged, this, [this]()
        {
            if (!m_map || m_map->loadStatus() != LoadStatus::Loaded)
                return;

            m_mapView->setMap(m_map);
            m_mapView->setVisible(true);
        }
    );

    m_map->load();
}

void ShowOrgBasemapPicker::errorAccepted()
{
    m_mapLoadeError.clear();
    emit mapLoadErrorChanged();
}

QString ShowOrgBasemapPicker::basemapTitle(int index)
{
    if (!basemaps())
        return "";

    Basemap* basemapAtIndex = m_portal->basemaps()->at(index);

    if (!basemapAtIndex)
        return "";

    return basemapAtIndex->item()->title();
}

QUrl ShowOrgBasemapPicker::basemapThumbnail(int index)
{
    if (!basemaps())
        return QUrl();

    Basemap* basemapAtIndex = m_portal->basemaps()->at(index);
    if (!basemapAtIndex)
        return QUrl();

    Item* itemAtIndex = basemapAtIndex->item();
    if (!itemAtIndex)
        return QUrl();

    QImage itemThumbnail = itemAtIndex->thumbnail();
    if (itemThumbnail.isNull())
    {
        itemAtIndex->fetchThumbnail();
    }
    else
    {
        itemThumbnail
    }
}

void ShowOrgBasemapPicker::onFetchBasemapsCompleted()
{
    emit basemapsChanged();
}

AuthenticationManager *ShowOrgBasemapPicker::authManager() const
{
    return AuthenticationManager::instance();
}
