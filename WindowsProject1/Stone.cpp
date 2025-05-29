#include "pch.h"
#include "Stone.h"
#include "TransformComponent.h"
#include "Camera3D.h"
#include "GameScene4.h"
#include "Physics3D.h"

Stone::Stone(ObjectManager* owner, ObjectType objType) : Object(owner, objType)
{
    transform = AddComponent<TransformComponent>();
    transform->SetRotationX(PI / 2.);

    camera = AddComponent<Camera3D>(transform);
    color = D3DXCOLOR(1.0, 0.0, 0.0, 1.0);
}

void Stone::Update(double dt)
{
    Object::Update(dt);
    for (auto stone : owner->GetObjectList(ObjectType::Front))
    {
        if(stone.get() != this) physics->ResolveCollision(stone->GetComponent<Physics3D>());
    }

    auto camOffset = camera->position - camera->target;
    camera->target = transform->GetPosition();
    camera->position = camera->target + camOffset;
}

void Stone::Render(LPDIRECT3DDEVICE9 device)
{
    device->SetRenderState(D3DRS_LIGHTING, TRUE); // ✅ 조명 켜기
    device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
    device->SetTexture(0, nullptr); // 텍스처 사용 안 함
    device->SetFVF(D3DFVF_XYZ | D3DFVF_NORMAL);
    D3DMATERIAL9 mtl = {};
    mtl.Diffuse = chosenStone == this ? D3DXCOLOR(0.0, 0.0, 0.3, 1.0) : color;
    mtl.Ambient = mtl.Diffuse;
    mtl.Specular = D3DXCOLOR(1, 1, 1, 1);
    mtl.Power = 20.0f; // 하이라이트 날카로움
    device->SetMaterial(&mtl);
    device->SetTransform(D3DTS_WORLD, &transform->GetWorldMatrix());

    cylinder->DrawSubset(0);
}
