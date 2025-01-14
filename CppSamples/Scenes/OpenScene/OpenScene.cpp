// [WriteFile Name=OpenScene, Category=Scenes]
// [Legal]
// Copyright 2018 Esri.
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
#include "OpenScene.h"

// ArcGIS Maps SDK headers
#include "PortalItem.h"
#include "Scene.h"
#include "SceneQuickView.h"

using namespace Esri::ArcGISRuntime;

OpenScene::OpenScene(QQuickItem* parent /* = nullptr */):
  QQuickItem(parent)
{
}

void OpenScene::init()
{
  // Register classes for QML
  qmlRegisterType<SceneQuickView>("Esri.Samples", 1, 0, "SceneView");
  qmlRegisterType<OpenScene>("Esri.Samples", 1, 0, "OpenSceneSample");
}

void OpenScene::componentComplete()
{
  QQuickItem::componentComplete();

  m_sceneView = findChild<SceneQuickView*>("sceneView");

  // Create a PortalItem with an Item ID
  PortalItem* item = new PortalItem(QStringLiteral("31874da8a16d45bfbc1273422f772270"), this);

  // Create a scene with the Item
  Scene* scene = new Scene(item, this);

  // Set the Scene on the SceneView
  m_sceneView->setArcGISScene(scene);
}
