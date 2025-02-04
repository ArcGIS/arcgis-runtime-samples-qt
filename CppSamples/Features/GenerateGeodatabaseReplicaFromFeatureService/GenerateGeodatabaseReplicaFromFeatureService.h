// [WriteFile Name=GenerateGeodatabaseReplicaFromFeatureService, Category=Features]
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

#ifndef GENERATE_GEODATABASE_H
#define GENERATE_GEODATABASE_H

// ArcGIS Maps SDK headers
#include "ArcGISFeatureServiceInfo.h"
#include "Envelope.h"
#include "GenerateGeodatabaseParameters.h"

// Qt headers
#include <QQuickItem>
#include <QStringList>
#include <QTemporaryDir>

namespace Esri::ArcGISRuntime
{
  class Map;
  class MapQuickView;
  class GeodatabaseSyncTask;
  class Geodatabase;
}

class GenerateGeodatabaseReplicaFromFeatureService : public QQuickItem
{
  Q_OBJECT

public:
  explicit GenerateGeodatabaseReplicaFromFeatureService(QQuickItem* parent = nullptr);
  ~GenerateGeodatabaseReplicaFromFeatureService() override;

  void componentComplete() override;
  static void init();
  Q_INVOKABLE void generateGeodatabaseFromCorners(double xCorner1, double yCorner1, double xCorner2, double yCorner2);

signals:
  void updateStatus(QString status);
  void hideWindow(int time, bool success);

private:
  void addFeatureLayers(const QString& serviceUrl, const QStringList& serviceIds);
  Esri::ArcGISRuntime::GenerateGeodatabaseParameters getUpdatedParameters(Esri::ArcGISRuntime::Envelope gdbExtent);
  void addOfflineData(Esri::ArcGISRuntime::Geodatabase* gdb);

private:
  Esri::ArcGISRuntime::Map* m_map = nullptr;
  Esri::ArcGISRuntime::MapQuickView* m_mapView = nullptr;
  Esri::ArcGISRuntime::GeodatabaseSyncTask* m_syncTask = nullptr;
  QString m_dataPath;
  QString m_featureServiceUrl = QStringLiteral("https://sampleserver6.arcgisonline.com/arcgis/rest/services/Sync/WildfireSync/FeatureServer/");
  QStringList m_serviceIds;
  Esri::ArcGISRuntime::ArcGISFeatureServiceInfo m_featureServiceInfo;
  QTemporaryDir m_tempPath;
};

#endif // GENERATE_GEODATABASE_H
