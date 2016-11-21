// [WriteFile Name=RasterLayerFile, Category=Analysis]
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

#ifndef RASTERLAYERFILE_H
#define RASTERLAYERFILE_H

namespace Esri
{
  namespace ArcGISRuntime
  {
    class MapQuickView;
  }
}

#include <QQuickItem>

class RasterLayerFile : public QQuickItem
{
  Q_OBJECT

public:
  RasterLayerFile(QQuickItem* parent = nullptr);
  ~RasterLayerFile();

  void componentComplete() Q_DECL_OVERRIDE;

  Q_INVOKABLE void createAndAddRasterLayer(QUrl rasterUrl);

private:
  Esri::ArcGISRuntime::MapQuickView* m_mapView;
};

#endif // RASTERLAYERFILE_H
