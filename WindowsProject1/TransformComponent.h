#pragma once
#include "ObjectComponent.h"

class TransformComponent : public ObjectComponent {
public:
    D3DXVECTOR3 vPosition{ 0.0f, 0.0f, 0.0f };
    float fRotationX = 0.0f;
    float fRotationY = 0.0f;
    float fRotationZ = 0.0f;
    D3DXVECTOR3 vScale{ 1.0f, 1.0f, 1.0f };

private:
    D3DXMATRIX matWorld;
    bool bDirty = true;

public:
    TransformComponent() = default;
    virtual ~TransformComponent() override = default;
    virtual void Initialize() override {}
    virtual void Update(double dt) override {}

    void SetPosition(float x, float y, float z = 0.0f) {
        vPosition = D3DXVECTOR3(x, y, z);
        bDirty = true;
    }

    void SetPosition(D3DXVECTOR3 vec) {
        vPosition = vec;
        bDirty = true;
    }

    void Translate(float dx, float dy, float dz = 0.0f) {
        vPosition += D3DXVECTOR3(dx, dy, dz);
        bDirty = true;
    }

    void Translate(D3DXVECTOR3 vec) {
        vPosition += vec;
        bDirty = true;
    }


    void SetRotationX(float angleRadians) {
        fRotationX = angleRadians;
        bDirty = true;
    }
    void RotateX(float deltaRadians) {
        fRotationX += deltaRadians;
        bDirty = true;
    }

    void SetRotationY(float angleRadians) {
        fRotationY = angleRadians;
        bDirty = true;
    }
    void RotateY(float deltaRadians) {
        fRotationY += deltaRadians;
        bDirty = true;
    }

    void SetRotationZ(float angleRadians) {
        fRotationZ = angleRadians;
        bDirty = true;
    }
    void RotateZ(float deltaRadians) {
        fRotationZ += deltaRadians;
        bDirty = true;
    }

    void SetScale(float sx, float sy, float sz = 1.0f) {
        vScale = D3DXVECTOR3(sx, sy, sz);
        bDirty = true;
    }
    void ScaleBy(float sx, float sy, float sz = 1.0f) {
        vScale.x *= sx;
        vScale.y *= sy;
        vScale.z *= sz;
        bDirty = true;
    }

    const D3DXVECTOR3& GetPosition() const { return vPosition; }
    float GetRotationX() const { return fRotationX; }
    float GetRotationY() const { return fRotationY; }
    float GetRotationZ() const { return fRotationZ; }
    const D3DXVECTOR3& GetScale() const { return vScale; }

    const D3DXMATRIX& GetWorldMatrix() {
        UpdateMatrix();
        return matWorld;
    }

private:
    void UpdateMatrix() {
        if (!bDirty) return;

        D3DXMATRIX matS, matR, matT;
        D3DXMATRIX matRX, matRY, matRZ;
        D3DXMatrixScaling(&matS, vScale.x, vScale.y, vScale.z);
        D3DXMatrixRotationX(&matRX, fRotationX);
        D3DXMatrixRotationY(&matRY, fRotationY);
        D3DXMatrixRotationZ(&matRZ, fRotationZ);
        D3DXMatrixTranslation(&matT, vPosition.x, vPosition.y, vPosition.z);

        matR = matRX * matRY * matRZ; // rotate x y z
        matWorld = matS * matR * matT; //scale rotate translate
        bDirty = false;
    }
};  