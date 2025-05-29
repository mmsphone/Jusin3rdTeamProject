#include "pch.h"
#include "Plate.h"
#include "TransformComponent.h"
#include "GameScene4.h"
#include "Camera3D.h"
Plate::Plate(ObjectManager* owner, ObjectType objType) : Object(owner, objType)
{
	transform = AddComponent<TransformComponent>();
    transform->SetScale(10.f, 1.f, 10.f);

    camera = AddComponent<Camera3D>(transform);
}

void Plate::Update(double dt)
{
}

void Plate::Render(LPDIRECT3DDEVICE9 device)
{
 
    device->SetRenderState(D3DRS_LIGHTING, TRUE); // ✅ 조명 켜기
    device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
    device->SetTexture(0, nullptr); // 텍스처 사용 안 함
    device->SetFVF(D3DFVF_XYZ | D3DFVF_NORMAL);
    D3DMATERIAL9 mtl = {};
    mtl.Diffuse = D3DXCOLOR(168/255., 76/255., 12/255., 1);
    mtl.Ambient = mtl.Diffuse;
    mtl.Specular = D3DXCOLOR(1, 1, 1, 1);
    mtl.Power = 20.0f; // 하이라이트 날카로움
    device->SetMaterial(&mtl);
    device->SetTransform(D3DTS_WORLD, &transform->GetWorldMatrix());

    box->DrawSubset(0);

    //D3DMATERIAL9 edgeMtl = {};
    //edgeMtl.Diffuse = D3DXCOLOR(0, 0, 0, 1);
    //edgeMtl.Ambient = edgeMtl.Diffuse;
    //device->SetMaterial(&edgeMtl);
    //device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
    //box->DrawSubset(0);
}
