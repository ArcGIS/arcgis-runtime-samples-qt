// Copyright 2015 Esri.

// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0

// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <QSettings>
#include <QGuiApplication>
#include <QQuickView>
#include <QCommandLineParser>
#include <QDir>
#include <QQmlEngine>

#ifdef Q_OS_WIN
#include <Windows.h>
#endif

#include "GOSymbols.h"

#define STRINGIZE(x) #x
#define QUOTE(x) STRINGIZE(x)

int main(int argc, char *argv[])
{
  QGuiApplication app(argc, argv);

#ifdef Q_OS_WIN
  // Force usage of OpenGL ES through ANGLE on Windows
  QCoreApplication::setAttribute(Qt::AA_UseOpenGLES);
#endif

  // Initialize the sample
  GOSymbols::init();

  // Intialize application view
  QQuickView view;
  view.setResizeMode(QQuickView::SizeRootObjectToView);

  // Add the import Path
  view.engine()->addImportPath(QDir(QCoreApplication::applicationDirPath()).filePath("qml"));
  // Add the Extras path
  view.engine()->addImportPath(QUOTE(ARCGIS_RUNTIME_IMPORT_PATH));
  // Add the Toolkit path
  view.engine()->addImportPath(QUOTE(ARCGIS_TOOLKIT_IMPORT_PATH));

  // Set the source
  view.setSource(QUrl("qrc:/Samples/DisplayInformation/GOSymbols/GOSymbols.qml"));

  view.show();

  return app.exec();
}


