#pragma once
#include "Scene.h"
class Camera3D;
class Stone;
inline Camera3D* mainCam;
inline Camera3D* originalCam;
inline Stone* chosenStone = nullptr;
class GameScene4 :
    public Scene
{
	float speed = 0.;
	const float maxSpeed = 30.f;
	enum class States
	{
		Idle,
		Pick,
		Charge,
		Shot,
		End,
	};
	States state;
public:
	LPDIRECT3D9 d3d = nullptr;
	LPDIRECT3DDEVICE9 d3dDevice = nullptr;
public:
	virtual void Load();
	virtual void Update(double dt);
	virtual void Render(HDC hdc);
	virtual void Unload();
	bool InitDirect3D(HWND hWnd);
	void CreatePrimitives();
    void CreateRayFromMouse(D3DXVECTOR3& outOrigin, D3DXVECTOR3& outDir);
    bool RayIntersectsTopCircle(const D3DXVECTOR3& rayOrigin, const D3DXVECTOR3& rayDir,
        const D3DXVECTOR3& center, float radius, float height, float& outT);
};
inline LPD3DXMESH cylinder = nullptr;
inline LPD3DXMESH box = nullptr;
inline LPD3DXLINE line = nullptr;

