#include "pch.h"
#include "Camera3D.h"
#include "TransformComponent.h"

Camera3D::Camera3D(TransformComponent* owner)
{
    target = owner->GetPosition();
    position = target + D3DXVECTOR3(-10.f, 10.f, -10.f); // 대각선 위쪽
    up = D3DXVECTOR3(0, 1, 0); // 정면 아래로 보는 카메라

    SetLens(D3DX_PI / 4.f, 4 / 3.f, 1.0f, 1000.0f);
    UpdateViewMatrix();
}

void Camera3D::SetLens(float fovY_, float aspect_, float zn_, float zf_)
{
    fovY = fovY_; aspect = aspect_; nearZ = zn_; farZ = zf_;
    D3DXMatrixPerspectiveFovLH(&projMatrix, fovY, aspect, nearZ, farZ);
}

void Camera3D::LookAt(const D3DXVECTOR3& pos, const D3DXVECTOR3& tgt, const D3DXVECTOR3& up_)
{
    position = pos;
    target = tgt;
    up = up_;
    UpdateViewMatrix();
}

void Camera3D::UpdateViewMatrix()
{
    D3DXMatrixLookAtLH(&viewMatrix, &position, &target, &up);
}