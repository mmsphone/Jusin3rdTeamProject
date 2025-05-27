#pragma once
class GameScene1 : public Scene
{
	virtual void Load();
	virtual void Update(double dt);
	virtual void Render(HDC hdc);
	virtual void Unload();

private:
	double count = 0.;

	double mainTimer = 0.;

	int iNumber;
};