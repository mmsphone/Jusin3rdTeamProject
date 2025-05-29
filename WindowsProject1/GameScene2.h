#pragma once
#include "Scene.h"
class GameScene2 :
    public Scene
{
	virtual void Load();
	virtual void Update(double dt);
	virtual void Render(HDC hdc);
	virtual void Unload();


private:
	double totalTime = 30.0; // 제한 시간 (초)
	double elapsedTime = 0.0;
	bool bTimeOver = false;
	int min;
	int sec;
};

