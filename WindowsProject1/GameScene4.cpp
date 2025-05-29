#include "pch.h"
#include "GameScene4.h"
#include "Engine.h"
#include "Plate.h"
#include "Stone.h"
#include "Camera3D.h"
#include "ObjectManager.h"
#include "TransformComponent.h"
#include "Physics3D.h"
#include "InputSystem.h"
#include "SoundManager.h"
#include <filesystem>



void GameScene4::Load()
{
	Scene::Load();
    InitDirect3D(Engine::GetInstance().GethWnd());
    CreatePrimitives();
    auto sm = Engine::GetInstance().GetSoundManager();

    sm->LoadSound("chooseStone", "../sound/chooseStone.wav", false);
    sm->LoadSound("considerStone", "../sound/considerStone.wav", false);
    sm->LoadSound("charge", "../sound/charge.wav", false);
    sm->LoadSound("shot", "../sound/shot.wav", false);
    sm->LoadSound("crash", "../sound/crash.wav", false);


    auto plate = std::make_shared<Plate>(objectManager.get(), ObjectType::Back);

    mainCam = plate->GetComponent<Camera3D>();
    originalCam = mainCam;
    objectManager->AddObject(ObjectType::Back, plate);
    
    float zCam = 2.0;
    float yCam = 0.7;
    // 1. 작은 말 7개 (병사)
    for (int i = 0; i < 7; i++)
    {
        auto stone = std::make_shared<Stone>(objectManager.get(), ObjectType::Front);
        stone->color = D3DXCOLOR(1.0, 0.0, 0.0, 1.0); // 한나라

        auto transform = stone->GetComponent<TransformComponent>();

        float x = (i - 3.f); // -3 ~ +3
        float z = 2.5f;

        transform->SetPosition(x, 0.5f + 0.2f * 0.5f, z);
        transform->SetScale(0.4f, 0.4f, 0.2f);

        auto camera = stone->GetComponent<Camera3D>();
        camera->LookAt(transform->GetPosition() + D3DXVECTOR3(0.0, yCam, zCam), transform->GetPosition(), D3DXVECTOR3(0.0, 1.0, 0.0));
        
        auto phyics3D = stone->AddComponent<Physics3D>(transform, camera, 0.15, 0.2);
        stone->physics = phyics3D;

        objectManager->AddObject(ObjectType::Front, stone);
    }

    // 2. 중간 말 8개 (차, 포, 상, 마)
    for (int i = 0; i < 8; i++)
    {
        auto stone = std::make_shared<Stone>(objectManager.get(), ObjectType::Front);
        stone->color = D3DXCOLOR(1.0, 0.0, 0.0, 1.0); // 한나라

        auto transform = stone->GetComponent<TransformComponent>();

        float x = (i - 3.5f); // -3.5 ~ +3.5
        float z = 1.5f;

        transform->SetPosition(x, 0.5f + 0.25f * 0.5f, z);
        transform->SetScale(0.7f, 0.7f, 0.25f);

        auto camera = stone->GetComponent<Camera3D>();
        camera->LookAt(transform->GetPosition() + D3DXVECTOR3(0.0, yCam, zCam), transform->GetPosition(), D3DXVECTOR3(0.0, 1.0, 0.0));
        auto phyics3D = stone->AddComponent<Physics3D>(transform, camera, 0.3, 0.35);
        stone->physics = phyics3D;
        objectManager->AddObject(ObjectType::Front, stone);
    }

    // 3. 왕 (1개)
    {
        auto stone = std::make_shared<Stone>(objectManager.get(), ObjectType::Front);
        stone->color = D3DXCOLOR(1.0, 0.0, 0.0, 1.0); // 한나라

        auto transform = stone->GetComponent<TransformComponent>();

        transform->SetPosition(0.0f, 0.5f + 0.3f * 0.5f, 4.0f);
        transform->SetScale(1.0f, 1.0f, 0.3f);

        auto camera = stone->GetComponent<Camera3D>();
        camera->LookAt(transform->GetPosition() + D3DXVECTOR3(0.0, yCam, zCam), transform->GetPosition(), D3DXVECTOR3(0.0, 1.0, 0.0));
        
        auto phyics3D = stone->AddComponent<Physics3D>(transform, camera, 1.0, 0.5);
        stone->physics = phyics3D;
        objectManager->AddObject(ObjectType::Front, stone);
    }
    
    // 1. 작은 말 7개 (병사)
    for (int i = 0; i < 7; i++)
    {
        auto stone = std::make_shared<Stone>(objectManager.get(), ObjectType::Front);
        stone->color = D3DXCOLOR(0.0, 0.3, 0.0, 1.0); // 초나라
        stone->isGreen = true;
        auto transform = stone->GetComponent<TransformComponent>();

        float x = (i - 3.f); // -3 ~ +3
        float z = -2.5f;

        transform->SetPosition(x, 0.5f + 0.2f * 0.5f, z);
        transform->SetScale(0.4f, 0.4f, 0.2f);

        auto camera = stone->GetComponent<Camera3D>();
        camera->LookAt(transform->GetPosition() + D3DXVECTOR3(0.0, yCam, -zCam), transform->GetPosition(), D3DXVECTOR3(0.0, 1.0, 0.0));
        
        auto phyics3D = stone->AddComponent<Physics3D>(transform, camera, 0.2, 0.2);
        stone->physics = phyics3D;
        objectManager->AddObject(ObjectType::Front, stone);
    }

    // 2. 중간 말 8개 (차, 포, 상, 마)
    for (int i = 0; i < 8; i++)
    {
        auto stone = std::make_shared<Stone>(objectManager.get(), ObjectType::Front);
        stone->color = D3DXCOLOR(0.0, 0.3, 0.0, 1.0); // 초나라
        stone->isGreen = true;

        auto transform = stone->GetComponent<TransformComponent>();

        float x = (i - 3.5f); // -3.5 ~ +3.5
        float z = -1.5f;

        transform->SetPosition(x, 0.5f + 0.25f * 0.5f, z);
        transform->SetScale(0.7f, 0.7f, 0.25f);

        auto camera = stone->GetComponent<Camera3D>();
        camera->LookAt(transform->GetPosition() + D3DXVECTOR3(0.0, yCam, -zCam), transform->GetPosition(), D3DXVECTOR3(0.0, 1.0, 0.0));
        
        auto phyics3D = stone->AddComponent<Physics3D>(transform, camera, 0.5, 0.35);
        stone->physics = phyics3D;
        
        objectManager->AddObject(ObjectType::Front, stone);
    }

    // 3. 왕 (1개)
    {
        auto stone = std::make_shared<Stone>(objectManager.get(), ObjectType::Front);
        stone->color = D3DXCOLOR(0.0, 0.3, 0.0, 1.0); // 초나라
        stone->isGreen = true;

        auto transform = stone->GetComponent<TransformComponent>();

        transform->SetPosition(0.0f, 0.5f + 0.3f * 0.5f, -4.0f);
        transform->SetScale(1.0f, 1.0f, 0.3f);

        auto camera = stone->GetComponent<Camera3D>();
        camera->LookAt(transform->GetPosition() + D3DXVECTOR3(0.0, yCam, -zCam), transform->GetPosition(), D3DXVECTOR3(0.0, 1.0, 0.0));
        
        auto phyics3D = stone->AddComponent<Physics3D>(transform, camera, 1.0, 0.5);
        stone->physics = phyics3D;

        objectManager->AddObject(ObjectType::Front, stone);
    }

    state = States::Idle;
}

void GameScene4::Update(double dt)
{

	objectManager->Update(dt);
    auto sm = Engine::GetInstance().GetSoundManager();

    auto inputSystem = Engine::GetInstance().GetInputSystem();

    switch (state)
    {
    case States::Idle:
    {

        if (inputSystem->IsMouseDown())
        {
            D3DXVECTOR3 RayOrigin, RayDir;
            CreateRayFromMouse(RayOrigin, RayDir);
            float closestT = FLT_MAX;
            for (auto s : objectManager->GetObjectList(ObjectType::Front))
            {
                auto stone = dynamic_cast<Stone*>(s.get());
                if (stone)
                {
                    float t = 0.f;
                    if (mainCam == originalCam && RayIntersectsTopCircle(RayOrigin, RayDir, stone->transform->GetPosition(), stone->physics->size, stone->transform->GetScale().z, t))
                    {
                        if (t < closestT)
                        {
                            closestT = t;
                            if (stone != chosenStone)
                            {
                                sm->StartSound("chooseStone");
                            }
                            chosenStone = stone;
                        }
                    }
                }
            }
            if (closestT == FLT_MAX)
            {
                mainCam = originalCam;
                if (chosenStone)
                {
                    sm->StartSound("chooseStone");
                }
                chosenStone = nullptr;
            }
        }
        

        if (chosenStone && inputSystem->IsKeyPressed(VK_SPACE))
        {
            mainCam = chosenStone->camera;
            state = States::Pick;
            sm->StartSound("considerStone");

        }
    } break;
    case States::Pick:
    {
        D3DXVECTOR3 target = chosenStone->GetComponent<TransformComponent>()->GetPosition();

        D3DXVECTOR3 camPos = mainCam->GetPosition();
        D3DXVECTOR3 offset = camPos - target; // 카메라 → 타겟 벡터
        offset.y = 0.f;
        float radius = D3DXVec3Length(&offset);

        // 현재 각도 계산 (XZ 평면)
        float angle = atan2f(offset.z, offset.x); // z 앞에 두는 게 LH 좌표계에서 맞음

        // 회전
        float angleDelta = PI * 0.5f; // 회전 속도 (라디안)
        if (inputSystem->IsKeyDown(VK_LEFT))
        {
            angle -= angleDelta * dt;
        }
        if (inputSystem->IsKeyDown(VK_RIGHT))
        {
            angle += angleDelta * dt;
        }

        // 새 위치 계산 (Y 높이는 그대로)
        D3DXVECTOR3 newPos = D3DXVECTOR3(
            target.x + cosf(angle) * radius,
            camPos.y,
            target.z + sinf(angle) * radius
        );

        // 적용
        mainCam->LookAt(newPos, target, D3DXVECTOR3(0, 1, 0));

        D3DXVECTOR3 dir = target - newPos;
        dir.y = 0.;
        D3DXVec3Normalize(&dir, &dir);

        if (inputSystem->IsKeyPressed(VK_SPACE))
        {
            mainCam = originalCam;
            chosenStone = nullptr;
            state = States::Idle;
            sm->StartSound("considerStone");
        }
        if (inputSystem->IsKeyPressed(VK_UP))
        {
            sm->StartSound("charge");

            state = States::Charge;
        }
    } break;
    case States::Charge:
    {
        if (inputSystem->IsKeyPressed(VK_SPACE))
        {
            mainCam = originalCam;
        }
        speed += 0.1;
        speed = std::min(speed, maxSpeed);

        if (inputSystem->IsKeyReleased(VK_UP) && speed > 0.f)
        {
            auto dir = (chosenStone->camera->target - chosenStone->camera->position);
            dir.y = 0.;
            chosenStone->physics->velocity = speed * dir;
            speed = 0.;
            state = States::Shot;
            sm->StopSound("charge");
            sm->StartSound("shot");

        }
    } break;
    case States::Shot:
    {
        if (inputSystem->IsKeyPressed(VK_SPACE))
        {
            mainCam = originalCam;
        }
        bool allStop = true;
        for (auto s : objectManager->GetObjectList(ObjectType::Front))
        {
            auto stone = dynamic_cast<Stone*>(s.get());

            if (stone && D3DXVec3LengthSq(&stone->physics->velocity) > 1.)
            {
                allStop = false;
            }
        }

        if (allStop)
        {
            mainCam = originalCam;
            state = States::End;
        }
    } break;
    case States::End:
    {
        static float timer = 0.0;
        timer += dt;

        if (timer > 2.f)
        {
            for (auto s : objectManager->GetObjectList(ObjectType::Front))
            {
                auto stone = dynamic_cast<Stone*>(s.get());
                if (stone)
                {
                    auto pos = stone->transform->GetPosition();
                    if (pos.x < -5 || pos.x > 5 || pos.z < -5 || pos.z > 5)
                    {
                        stone->SetDead();
                    }
                }
            }
            timer = 0.f;
            chosenStone = nullptr;
            state = States::Idle;
        }
    } break;
    }
}

void GameScene4::Render(HDC hdc)
{
    if (!d3dDevice) return;

    d3dDevice->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
        D3DCOLOR_XRGB(100, 100, 255), 1.0f, 0);
    
    D3DLIGHT9 light = {};
    light.Type = D3DLIGHT_DIRECTIONAL;
    light.Direction = D3DXVECTOR3(0, -1, 0);
    light.Diffuse = D3DXCOLOR(1, 1, 1, 1);

    d3dDevice->SetLight(0, &light);
    d3dDevice->LightEnable(0, TRUE);

    d3dDevice->SetTransform(D3DTS_VIEW, &mainCam->GetViewMatrix());
    d3dDevice->SetTransform(D3DTS_PROJECTION, &mainCam->GetProjMatrix());

    d3dDevice->BeginScene();

    objectManager->Render(d3dDevice); // DX9 기반 렌더링

    if (state == States::Pick || state == States::Charge)
    {
        auto dir = (chosenStone->camera->target - chosenStone->camera->position);
        dir.y = 0.;
        D3DXVECTOR3 lineVerts[] = { chosenStone->transform->GetPosition(), chosenStone->transform->GetPosition() + dir * 3.f};
        line->Begin();
        D3DXMATRIX wvp = mainCam->GetViewMatrix() * mainCam->GetProjMatrix();  // 월드는 단위행렬이면 생략 가능
        float speedNorm = speed / maxSpeed; // 0.0 ~ 1.0 범위로 정규화
        speedNorm = std::min(speedNorm, 1.0f);
        D3DXCOLOR color;
        if (speedNorm < 0.5f) {
            float t = speedNorm / 0.5f;
            color = D3DXCOLOR(1.0f * t, 1.0f, 0.0f, 1.0f);  // (0,1,0) → (1,1,0)
        }
        else {
            float t = (speedNorm - 0.5f) / 0.5f;
            color = D3DXCOLOR(1.0f, 1.0f - t, 0.0f, 1.0f);  // (1,1,0) → (1,0,0)
        }
        line->DrawTransform(lineVerts, 2, &wvp, color);
        line->End();
    }

    d3dDevice->EndScene();
    d3dDevice->Present(nullptr, nullptr, nullptr, nullptr);

}

void GameScene4::Unload()
{
    objectManager->ClearAll();
    if (d3dDevice) d3dDevice->Release();
    if (d3d) d3d->Release();
}

bool GameScene4::InitDirect3D(HWND hWnd) {
    d3d = Direct3DCreate9(D3D_SDK_VERSION);
    if (!d3d) return false;

    D3DPRESENT_PARAMETERS d3dpp = {};
    d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.hDeviceWindow = hWnd;
    d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
    d3dpp.EnableAutoDepthStencil = TRUE;
    d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
    HRESULT hr = d3d->CreateDevice(
        D3DADAPTER_DEFAULT,
        D3DDEVTYPE_HAL,
        hWnd,
        D3DCREATE_SOFTWARE_VERTEXPROCESSING,
        &d3dpp,
        &d3dDevice
    );

    return SUCCEEDED(hr);
}

void GameScene4::CreatePrimitives()
{
    D3DXCreateCylinder(d3dDevice,
        0.5f,  // bottom radius
        0.5f,  // top radius
        1.0f,  // height
        20,    // slices
        1,     // stacks
        &cylinder,
        nullptr);

    D3DXCreateBox(d3dDevice,
        1.0f,   // width
        1.0f,   // height
        1.0f,   // depth
        &box,
        nullptr);

    D3DXCreateLine(d3dDevice, &line);
}

void GameScene4::CreateRayFromMouse(D3DXVECTOR3& outOrigin, D3DXVECTOR3& outDir)
{
    auto mousePos = Engine::GetInstance().GetInputSystem()->GetMousePos();
    float mouseX = mousePos.x;
    float mouseY = mousePos.y;
    auto screenSize = Engine::GetInstance().GetWindowSize();

    // NDC 좌표 [-1,1]
    float ndcX = (2.0f * mouseX / screenSize.right - 1.0f);
    float ndcY = -(2.0f * mouseY / screenSize.bottom - 1.0f); // 좌표계 반전

    D3DXMATRIX invView, invProj;
    D3DXMatrixInverse(&invView, nullptr, &mainCam->GetViewMatrix());
    D3DXMatrixInverse(&invProj, nullptr, &mainCam->GetProjMatrix());

    D3DXVECTOR3 rayNDC(ndcX, ndcY, 1.0f); // Z=1 (원근 투영)
    D3DXVECTOR3 rayView;
    D3DXVec3TransformCoord(&rayView, &rayNDC, &invProj);
    D3DXVec3Normalize(&rayView, &rayView);

    outOrigin = D3DXVECTOR3(0, 0, 0);
    D3DXVec3TransformCoord(&outOrigin, &outOrigin, &invView);

    D3DXVec3TransformNormal(&outDir, &rayView, &invView);
    D3DXVec3Normalize(&outDir, &outDir);
}

bool GameScene4::RayIntersectsTopCircle(const D3DXVECTOR3& rayOrigin, const D3DXVECTOR3& rayDir, const D3DXVECTOR3& center, float radius, float height, float& outT)
{
    // 평면 Y = center.y + height / 2
    float yPlane = center.y + height * 0.5f;

    // 평면과 광선 교차: t = (planeY - rayOriginY) / rayDirY
    if (fabs(rayDir.y) < 1e-6f) return false; // 수평이면 무시
    float t = (yPlane - rayOrigin.y) / rayDir.y;
    if (t < 0) return false; // 뒤쪽

    // 교차점 계산
    D3DXVECTOR3 hit = rayOrigin + rayDir * t;

    // hit가 원 안에 있는지 검사 (XZ 평면상 거리)
    float dx = hit.x - center.x;
    float dz = hit.z - center.z;
    if (dx * dx + dz * dz <= radius * radius)
    {
        outT = t;
        return true;
    }

    return false;
}
