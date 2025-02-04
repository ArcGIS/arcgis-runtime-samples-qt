// [WriteFile Name=DisplayDeviceLocationWithNmeaDataSources, Category=Maps]
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

#ifndef DISPLAYDEVICELOCATIONWITHNMEADATASOURCES_H
#define DISPLAYDEVICELOCATIONWITHNMEADATASOURCES_H

// Qt headers
#include <QByteArray>
#include <QFile>
#include <QObject>
#include <QTimer>

namespace Esri::ArcGISRuntime
{
class Map;
class MapQuickView;
class NmeaLocationDataSource;
}

Q_MOC_INCLUDE("MapQuickView.h")

class DisplayDeviceLocationWithNmeaDataSources : public QObject
{
  Q_OBJECT

  Q_PROPERTY(Esri::ArcGISRuntime::MapQuickView* mapView READ mapView WRITE setMapView NOTIFY mapViewChanged)
  Q_PROPERTY(bool nmeaSimulationActive MEMBER m_nmeaSimulationActive NOTIFY nmeaSimulationActiveChanged)

public:
  explicit DisplayDeviceLocationWithNmeaDataSources(QObject* parent = nullptr);
  ~DisplayDeviceLocationWithNmeaDataSources();

  static void init();

  Q_INVOKABLE void start();
  Q_INVOKABLE void reset();

signals:
  void mapViewChanged();
  void nmeaSimulationActiveChanged();

private:
  Esri::ArcGISRuntime::MapQuickView* mapView() const;
  void setMapView(Esri::ArcGISRuntime::MapQuickView* mapView);
  bool loadMockDataFile(const QString& filePath);

  Esri::ArcGISRuntime::Map* m_map = nullptr;
  Esri::ArcGISRuntime::MapQuickView* m_mapView = nullptr;
  Esri::ArcGISRuntime::NmeaLocationDataSource* m_nmeaLocationDataSource = nullptr;
  QList<QByteArray> m_mockNmeaSentences;
  int m_mockDataIterator;
  QTimer* m_timer = nullptr;
  bool m_nmeaSimulationActive = false;
};

#endif // DISPLAYDEVICELOCATIONWITHNMEADATASOURCES_H
