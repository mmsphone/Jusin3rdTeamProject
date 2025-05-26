#include "pch.h"
#include "SceneManager.h"
#include "Scene.h"
#include "GameScene.h"

void SceneManager::Initialize()
{
	AddScene("Game", std::make_shared<GameScene>());
	SetActiveScene("Game");
}

void SceneManager::Update(double dt)
{
	activeScene->Update(dt);
}

void SceneManager::Render(HDC hdc)
{
	activeScene->Render(hdc);
}

void SceneManager::AddScene(const std::string& name, std::shared_ptr<Scene> scene)
{
	scenes[name] = scene;
}

void SceneManager::SetActiveScene(const std::string& name) {
    auto it = scenes.find(name);
    if (it != scenes.end()) {
        activeScene = it->second;
		activeScene->Load();
    }
}
