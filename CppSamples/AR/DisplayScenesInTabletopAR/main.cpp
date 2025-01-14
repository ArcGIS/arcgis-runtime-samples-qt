// [Legal]
// Copyright 2020 Esri.
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

// sample headers
#include "DisplayScenesInTabletopAR.h"

// toolkit headers
#include "ArcGISArView.h"

// ArcGIS Maps SDK headers
#include "ArcGISRuntimeEnvironment.h"

// Qt headers
#include <QDir>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickWindow>
#include <QSurfaceFormat>

// Platform specific headers
#ifdef Q_OS_WIN
#include <Windows.h>
#endif

#define STRINGIZE(x) #x
#define QUOTE(x) STRINGIZE(x)

int main(int argc, char *argv[])
{
  // At this time AR with the ArcGIS Maps SDK for Qt only supports OpenGL
  QQuickWindow::setGraphicsApi(QSGRendererInterface::GraphicsApi::OpenGL);

  // There are some conflicts between the AR frameworks and Qt's rendering thread.
  // See Qt's documentation about non-threaded render loops for more information.
  // https://doc.qt.io/qt-5/qtquick-visualcanvas-scenegraph.html#non-threaded-render-loops-basic-and-windows
#if defined(Q_OS_IOS) || defined(Q_OS_ANDROID)
  qputenv("QSG_RENDER_LOOP", "basic");
#endif

#if defined(Q_OS_LINUX) && !defined(Q_OS_ANDROID)
  // Linux requires 3.2 OpenGL Context
  // in order to instance 3D symbols
  QSurfaceFormat fmt = QSurfaceFormat::defaultFormat();
  fmt.setVersion(3, 2);
  QSurfaceFormat::setDefaultFormat(fmt);
#endif

  QGuiApplication app(argc, argv);
  app.setApplicationName(QString("DisplayScenesInTabletopAR"));

  // Use of ArcGIS location services, such as basemap styles, geocoding, and routing services,
  // requires an access token. For more information see
  // https://links.esri.com/arcgis-runtime-security-auth.

  // The following methods grant an access token:

  // 1. User authentication: Grants a temporary access token associated with a user's ArcGIS account.
  // To generate a token, a user logs in to the app with an ArcGIS account that is part of an
  // organization in ArcGIS Online or ArcGIS Enterprise.

  // 2. API key authentication: Get a long-lived access token that gives your application access to
  // ArcGIS location services. Go to the tutorial at https://links.esri.com/create-an-api-key.
  // Copy the API Key access token.

  const QString accessToken = QString("");

  if (accessToken.isEmpty())
  {
      qWarning() << "Use of ArcGIS location services, such as the basemap styles service, requires" <<
                    "you to authenticate with an ArcGIS account or set the API Key property.";
  }
  else
  {
      Esri::ArcGISRuntime::ArcGISRuntimeEnvironment::setApiKey(accessToken);
  }

  // Initialize the sample
  DisplayScenesInTabletopAR::init();

  QString arcGISRuntimeImportPath = QUOTE(ARCGIS_RUNTIME_IMPORT_PATH);

#if defined(LINUX_PLATFORM_REPLACEMENT)
  // on some linux platforms the string 'linux' is replaced with 1
  // fix the replacement paths which were created
  QString replaceString = QUOTE(LINUX_PLATFORM_REPLACEMENT);
  arcGISRuntimeImportPath = arcGISRuntimeImportPath.replace(replaceString, "linux", Qt::CaseSensitive);
#endif

  Esri::ArcGISRuntime::Toolkit::ArcGISArView::qmlRegisterTypes();

  // Initialize application view
  QQmlApplicationEngine engine;
  // Add the import Path
  engine.addImportPath(QDir(QCoreApplication::applicationDirPath()).filePath("qml"));
  // Add the Runtime and Extras path
  engine.addImportPath(arcGISRuntimeImportPath);

  // Set the source
  engine.load(QUrl("qrc:/Samples/AR/DisplayScenesInTabletopAR/main.qml"));

  return app.exec();
}
