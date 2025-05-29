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

    
    float friction = 0.97f; // 0.95 ~ 0.99 ���� ��õ

    // �� �����Ӹ���:
    velocity *= friction;

    // �ſ� �������ٸ� ���� �ɷ� ����
    if (D3DXVec3LengthSq(&velocity) < 0.0001f)
        velocity = D3DXVECTOR3(0, 0, 0);
}

void Physics3D::ResolveCollision(Physics3D* other)
{

    // �� A
    D3DXVECTOR3 posA = transform->GetPosition();
    D3DXVECTOR3& velA = velocity;
    float radiusA = size;
    float massA = mass;

    // �� B
    D3DXVECTOR3 posB = other->transform->GetPosition();
    D3DXVECTOR3& velB = other->velocity;
    float radiusB = other->size;
    float massB = other->mass;
    
    // ���� ���� ����
    D3DXVECTOR3 n = posB - posA;
    float distSq = n.x * n.x + n.z * n.z; // Y ����
    float minDist = radiusA + radiusB;
    if (distSq >= minDist * minDist) return; // �浹���� ����

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

    if (D3DXVec3LengthSq(&velB) < 0.0001f) // ������ ��
    {
        float speedA = D3DXVec3Length(&velA);
        D3DXVec3Normalize(&velA, &velA); // A�� ����
        float dot = D3DXVec3Dot(&velA, &n);

        D3DXVECTOR3 reflectA = velA - 2 * dot * n;

        float totalMass = massA + massB;
        float ratioB = massA / totalMass;
        float ratioA = massB / totalMass;

        velB = n * speedA * ratioB;
        velA = reflectA * speedA * ratioA;

        return;
    }


    // ��� �ӵ�
    D3DXVECTOR3 relVel = velA - velB;
    float relVelAlongNormal = D3DXVec3Dot(&relVel, &n);
    if (relVelAlongNormal >= 0) return; // �̹� �־����� ������ ����

    // �浹 ����: ź�� ��� 1.0 (���� ź��)
    float e = 1.0f;

    // ��� ���� ����
    float j = -(1 + e) * relVelAlongNormal;
    j /= (1 / massA + 1 / massB);

    // ��ݷ� ����
    D3DXVECTOR3 impulse = j * n;

    // �ӵ� ����
    velA += impulse / massA;
    velB -= impulse / massB;
}
