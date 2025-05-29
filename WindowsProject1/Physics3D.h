#pragma once
#include "ObjectComponent.h"
class TransformComponent;
class Camera3D;
class Physics3D : public ObjectComponent {
    TransformComponent* transform;
    Camera3D* cam;
public:
    float mass = 1.0f;
    float size = 0.f;
    D3DXVECTOR3 velocity = D3DXVECTOR3(0, 0, 0); // Y축은 무시 가능
    Physics3D(TransformComponent* transform, Camera3D* cam, float mass, float size);

    virtual void Initialize() {};
    virtual void Update(double dt);
    void ResolveCollision(Physics3D* other);
};