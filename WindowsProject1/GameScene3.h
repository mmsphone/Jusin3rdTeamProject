#pragma once
#include "Scene.h"
class GameScene3 :
    public Scene
{
	virtual void Load();
	virtual void Update(double dt);
	virtual void Render(HDC hdc);
	virtual void Unload();
};

