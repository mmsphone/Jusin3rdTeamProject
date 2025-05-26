#pragma once
#include "ObjectManager.h"

class Scene
{
public:
	Scene() = default;
	virtual void Load();
	virtual void Unload() = 0;
	virtual void Render(HDC) = 0;
	virtual void Update(double deltaTime) = 0;

public:
	std::unique_ptr<ObjectManager> objectManager;
};