#pragma once
#include "Scene.h"
class GameScene4 :
    public Scene
{
	virtual void Load();
	virtual void Update(double dt);
	virtual void Render(HDC hdc);
	virtual void Unload();
};

