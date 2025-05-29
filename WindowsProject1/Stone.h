#pragma once
#include "Object.h"
class TransformComponent;
class Camera3D;
class Physics3D;
class Stone : public Object
{
public:
	Physics3D* physics;
	Camera3D* camera;
	TransformComponent* transform;
	bool chosen = false;
	bool isGreen = false;
public:
	Stone(ObjectManager* owner, ObjectType objType);
	D3DXCOLOR color;

	virtual void Update(double dt);
	virtual void Render(LPDIRECT3DDEVICE9);
private:

};