#pragma once
#include "Object.h"
class TransformComponent;
class Camera3D;
class Plate : public Object
{
	Camera3D* camera;
	TransformComponent* transform;

public:
	Plate(ObjectManager* owner, ObjectType objType);

	virtual void Update(double dt);
	virtual void Render(LPDIRECT3DDEVICE9);
private:
};