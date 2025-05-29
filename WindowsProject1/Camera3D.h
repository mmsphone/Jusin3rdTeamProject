#pragma once
#include "ObjectComponent.h"
class TransformComponent;
class Camera3D : public ObjectComponent {
public:
    D3DXVECTOR3 position;
    D3DXVECTOR3 target;
    D3DXVECTOR3 up;

    float fovY = D3DX_PI / 4;
    float aspect;
    float nearZ;
    float farZ;

    D3DXMATRIX viewMatrix;
    D3DXMATRIX projMatrix;

public:
    Camera3D(TransformComponent* owner);
    virtual void Initialize() {};
    virtual void Update(double dt) {};
    void SetLens(float fovY, float aspect, float zn, float zf);
    void LookAt(const D3DXVECTOR3& pos, const D3DXVECTOR3& target, const D3DXVECTOR3& up);
    void UpdateViewMatrix();

    const D3DXMATRIX& GetViewMatrix() const { return viewMatrix; }
    const D3DXMATRIX& GetProjMatrix() const { return projMatrix; }

    D3DXVECTOR3 GetPosition() const { return position; }

    // 프러스텀이나 충돌용 AABB 관련 기능도 여기에 추가 가능
};