#include "pch.h"
#include "Physics3D.h"
#include "Camera3D.h"
#include "TransformComponent.h"

#include "Engine.h"
#include "SoundManager.h"
Physics3D::Physics3D(TransformComponent* transform, Camera3D* cam, float mass, float size) : transform(transform), cam(cam), mass(mass), size(size)
{
}

void Physics3D::Update(double dt) 
{
    velocity.y = 0.f;
    transform->Translate(velocity * dt);

    
    float friction = 0.97f; // 0.95 ~ 0.99 사이 추천

    // 매 프레임마다:
    velocity *= friction;

    // 매우 느려졌다면 멈춘 걸로 간주
    if (D3DXVec3LengthSq(&velocity) < 0.0001f)
        velocity = D3DXVECTOR3(0, 0, 0);
}

void Physics3D::ResolveCollision(Physics3D* other)
{

    // 돌 A
    D3DXVECTOR3 posA = transform->GetPosition();
    D3DXVECTOR3& velA = velocity;
    float radiusA = size;
    float massA = mass;

    // 돌 B
    D3DXVECTOR3 posB = other->transform->GetPosition();
    D3DXVECTOR3& velB = other->velocity;
    float radiusB = other->size;
    float massB = other->mass;
    
    // 방향 단위 벡터
    D3DXVECTOR3 n = posB - posA;
    float distSq = n.x * n.x + n.z * n.z; // Y 무시
    float minDist = radiusA + radiusB;
    if (distSq >= minDist * minDist) return; // 충돌하지 않음

    D3DXVec3Normalize(&n, &n);
    Engine::GetInstance().GetSoundManager()->StartSound("crash");

    float dist = sqrtf(distSq);
    float penetration = minDist - dist;

    float totalMass = massA + massB;
    float ratioA = massB / totalMass;
    float ratioB = massA / totalMass;

    D3DXVECTOR3 correction = n * penetration;

    transform->SetPosition(posA - correction * ratioA);
    other->transform->SetPosition(posB + correction * ratioB);

    if (D3DXVec3LengthSq(&velB) < 0.0001f) // 정지한 돌
    {
        float speedA = D3DXVec3Length(&velA);
        D3DXVec3Normalize(&velA, &velA); // A의 방향
        float dot = D3DXVec3Dot(&velA, &n);

        D3DXVECTOR3 reflectA = velA - 2 * dot * n;

        float totalMass = massA + massB;
        float ratioB = massA / totalMass;
        float ratioA = massB / totalMass;

        velB = n * speedA * ratioB;
        velA = reflectA * speedA * ratioA;

        return;
    }


    // 상대 속도
    D3DXVECTOR3 relVel = velA - velB;
    float relVelAlongNormal = D3DXVec3Dot(&relVel, &n);
    if (relVelAlongNormal >= 0) return; // 이미 멀어지고 있으면 무시

    // 충돌 반응: 탄성 계수 1.0 (완전 탄성)
    float e = 1.0f;

    // 운동량 보존 공식
    float j = -(1 + e) * relVelAlongNormal;
    j /= (1 / massA + 1 / massB);

    // 충격량 벡터
    D3DXVECTOR3 impulse = j * n;

    // 속도 갱신
    velA += impulse / massA;
    velB -= impulse / massB;
}
