// [WriteFile Name=IntegratedWindowsAuthentication, Category=CloudAndPortal]
// [Legal]
// Copyright 2019 Esri.
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

#ifndef INTEGRATEDWINDOWSAUTHENTICATION_H
#define INTEGRATEDWINDOWSAUTHENTICATION_H

// Qt headers
#include <QAbstractListModel>
#include <QObject>

namespace Esri::ArcGISRuntime
{
class Map;
class MapQuickView;
class Portal;
class PortalItem;
class PortalQueryResultSetForItems;
class PortalItemListModel;
class PortalQueryParametersForItems;
class PortalQueryResultSetForItems;
class Error;
}

Q_MOC_INCLUDE("MapQuickView.h")

class IntegratedWindowsAuthentication : public QObject
{
  Q_OBJECT

  Q_PROPERTY(Esri::ArcGISRuntime::MapQuickView* mapView READ mapView WRITE setMapView NOTIFY mapViewChanged)
  Q_PROPERTY(QAbstractListModel* webmapListModel READ webmapListModel NOTIFY webmapListModelChanged)
  Q_PROPERTY(QString mapLoadError MEMBER m_mapLoadError NOTIFY mapLoadErrorChanged)
  Q_PROPERTY(bool isLoading MEMBER m_loadingIndicator NOTIFY isLoadingChanged)

public:
  explicit IntegratedWindowsAuthentication(QObject* parent = nullptr);
  ~IntegratedWindowsAuthentication();

  static void init();

  Q_INVOKABLE void searchIwaSecurePortal(const QString& url);
  Q_INVOKABLE void loadSelectedWebmap(int index);
  Q_INVOKABLE void errorAccepted();
  QAbstractListModel* webmapListModel() const;

signals:
  void mapViewChanged();
  void webmapListModelChanged();
  void mapLoadErrorChanged();
  void isLoadingChanged();

private slots:
  void securePortalDoneLoading(const Esri::ArcGISRuntime::Error& loadError);
  void searchItemsCompleted(Esri::ArcGISRuntime::PortalQueryResultSetForItems* result);

private:
  Esri::ArcGISRuntime::MapQuickView* mapView() const;
  void setMapView(Esri::ArcGISRuntime::MapQuickView* mapView);

  Esri::ArcGISRuntime::Map* m_map = nullptr;
  Esri::ArcGISRuntime::MapQuickView* m_mapView = nullptr;
  Esri::ArcGISRuntime::PortalQueryResultSetForItems* m_webmapResults = nullptr;
  Esri::ArcGISRuntime::PortalItemListModel* m_webmaps = nullptr;
  Esri::ArcGISRuntime::PortalItem* m_selectedItem = nullptr;
  Esri::ArcGISRuntime::Portal* m_iwaSecurePortal = nullptr;
  Esri::ArcGISRuntime::PortalQueryParametersForItems* query = nullptr;
  QString m_mapLoadError;
  bool m_loadingIndicator = false;
};

#endif // INTEGRATEDWINDOWSAUTHENTICATION_H
