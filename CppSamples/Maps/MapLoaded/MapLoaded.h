// [WriteFile Name=MapLoaded, Category=Maps]
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

#ifndef MAP_LOADED_H
#define MAP_LOADED_H

// Qt headers
#include <QQuickItem>

namespace Esri::ArcGISRuntime
{
  class Map;
  class MapQuickView;
}

class QString;

class MapLoaded : public QQuickItem
{
  Q_OBJECT

  Q_PROPERTY(QString mapLoadStatus READ mapLoadStatus NOTIFY mapLoadStatusChanged)

public:
  explicit MapLoaded(QQuickItem* parent = nullptr);
  ~MapLoaded() override;

  void componentComplete() override;
  static void init();

signals:
  void mapLoadStatusChanged();

private:
  QString mapLoadStatus() const;

private:
  Esri::ArcGISRuntime::Map* m_map = nullptr;
  Esri::ArcGISRuntime::MapQuickView* m_mapView = nullptr;
  QString m_loadStatus;
};

#endif // MAP_LOADED_H
