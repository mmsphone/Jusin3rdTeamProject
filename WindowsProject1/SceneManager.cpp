#include "pch.h"
#include "SceneManager.h"
#include "Scene.h"
#include "GameScene1.h"
#include "GameScene2.h"
#include "GameScene3.h"
#include "GameScene4.h"
#include "MenuScene.h"

void SceneManager::Initialize()
{
	AddScene("Menu", std::make_shared<MenuScene>());
	AddScene("Game1", std::make_shared<GameScene1>());
	AddScene("Game2", std::make_shared<GameScene2>());
	AddScene("Game3", std::make_shared<GameScene3>());
	AddScene("Game4", std::make_shared<GameScene4>());
	SetActiveScene("Menu");
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
