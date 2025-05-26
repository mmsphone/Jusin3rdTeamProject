#include "pch.h"
#include "Scene.h"

void Scene::Load()
{
	objectManager = std::make_unique<ObjectManager>();
	objectManager->Initialize();
}
