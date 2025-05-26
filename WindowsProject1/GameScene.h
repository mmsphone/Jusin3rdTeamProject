#pragma once
class GameScene : public Scene
{
	virtual void Load();
	virtual void Update(double dt);
	virtual void Render(HDC hdc);
	virtual void Unload();

private:
	double count = 0.;

	double mainTimer = 0.;
};