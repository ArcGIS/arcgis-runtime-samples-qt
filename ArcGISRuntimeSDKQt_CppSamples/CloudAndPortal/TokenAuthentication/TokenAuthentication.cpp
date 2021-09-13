// [WriteFile Name=TokenAuthentication, Category=CloudAndPortal]
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

#ifdef PCH_BUILD
#include "pch.hpp"
#endif // PCH_BUILD

#include "TokenAuthentication.h"

#include "Map.h"
#include "MapQuickView.h"
#include "Portal.h"
#include "PortalItem.h"

#include <QUrl>

using namespace Esri::ArcGISRuntime;

TokenAuthentication::TokenAuthentication(QQuickItem* parent /* = nullptr */):
  QQuickItem(parent)
{
}

TokenAuthentication::~TokenAuthentication() = default;

void TokenAuthentication::init()
{
  qmlRegisterType<MapQuickView>("Esri.Samples", 1, 0, "MapView");
  qmlRegisterType<TokenAuthentication>("Esri.Samples", 1, 0, "TokenAuthenticationSample");
}

void TokenAuthentication::componentComplete()
{
  QQuickItem::componentComplete();

  // find QML MapView component
  m_mapView = findChild<MapQuickView*>("mapView");
  if (!m_mapView)
    return;

  m_mapView->setWrapAroundMode(WrapAroundMode::Disabled);

  m_portal = new Portal(m_portalURL, this);
  m_portalItem =  new PortalItem(m_portal, m_itemID, this);

  m_map = new Map(m_portalItem, this);

  // Set map to map view
  m_mapView->setMap(m_map);
}
