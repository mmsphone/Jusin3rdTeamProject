#include "pch.h"
#include "Boss.h"
#include "ObjectManager.h"
#include "Bullet.h"
#include "TransformComponent.h"
#include "Engine.h"

Boss::Boss(ObjectManager* owner, ObjectType objType, double speed) : Object(owner, objType, RenderType::Rect), speed(speed), clockwise(true), currArea(0)
{
	auto transform = AddComponent<TransformComponent>();
	transform->SetScale(100, 300);
	RECT windowSize = Engine::GetInstance().GetWindowSize();

	math::vec2 center{ windowSize.right * 3. / 4., windowSize.bottom / 2. };

	transform->SetPosition(center);
	ChangeState(&bossIdle);

	area[0] = math::vec2{ windowSize.right * 3. / 4., windowSize.bottom / 4. }; //top
	area[1] = math::vec2{ windowSize.right * 7. / 8., windowSize.bottom / 2. }; //right
	area[2] = math::vec2{ windowSize.right * 3. / 4., windowSize.bottom * 3 / 4. }; //bottom
	area[3] = math::vec2{ windowSize.right * 5. / 8., windowSize.bottom / 2. }; //left
}

void Boss::Update(double dt)
{
	Object::Update(dt);
	auto transform = GetComponent<TransformComponent>();
	transform->Translate(0, speed * dt);


}

void Boss::BossIdle::Enter(Object*)
{
	timer = 0.;
}

void Boss::BossIdle::Update(Object*, double dt)
{
	timer += dt;
}

void Boss::BossIdle::TestForExit(Object* owner)
{
	auto boss = dynamic_cast<Boss*>(owner);
	if (timer > ATTACK_COOLDOWN)
	{
		if(rand() % 2) boss->ChangeState(&boss->bossAttack1);
		else boss->ChangeState(&boss->bossAttack2);
	}
}

void Boss::BossAttack1::Enter(Object* owner)
{
	timer = 0.;
	bulletNumber = 0;
	startPos = owner->GetComponent<TransformComponent>()->GetPosition();
	startAngle = rand() / PI;
}

void Boss::BossAttack1::Update(Object* owner, double dt)
{
	timer += dt;
	if (timer > BULLET_COOLDOWN)
	{
		timer = 0.;
		auto boss = dynamic_cast<Boss*>(owner);
		auto bullet = std::make_shared<Bullet>(owner->GetOwner(), ObjectType::EBullet, 50.);
		auto bulletTransform = bullet->AddComponent<TransformComponent>();
		double rotate = startAngle + bulletNumber * 0.2;
		math::vec2 startPositionGap = { START_POS_OFFSET * cos(rotate) + START_POS_OFFSET * sin(rotate), START_POS_OFFSET * cos(rotate) - START_POS_OFFSET * sin(rotate) };

		bulletTransform->SetPosition(startPos.x + startPositionGap.x, startPos.y + startPositionGap.y);
		bulletTransform->SetScale(20., 20.);
		bulletTransform->SetRotation(rotate);
		boss->GetOwner()->AddObject(ObjectType::EBullet, bullet);
		bulletNumber++;
	}
}

void Boss::BossAttack1::TestForExit(Object* owner)
{
	auto boss = dynamic_cast<Boss*>(owner);
	if (bulletNumber == TOTAL_BULLET_NUMBER)
	{
		boss->ChangeState(&boss->bossIdle);
	}
}

void Boss::BossAttack2::Enter(Object* owner)
{
	timer = 0.;
	bulletNumber = 0;
	startPos = owner->GetComponent<TransformComponent>()->GetPosition();
	auto playerPosition = owner->GetOwner()->GetFrontObject(ObjectType::Player)->GetComponent<TransformComponent>()->GetPosition();
	angle = std::atan2(-(playerPosition.y - startPos.y), playerPosition.x - startPos.x) - PI / 3;
}

void Boss::BossAttack2::Update(Object* owner, double dt)
{
	timer += dt;
	if (timer > BULLET_COOLDOWN)
	{
		timer = 0.;
		auto boss = dynamic_cast<Boss*>(owner);

		auto bullet = std::make_shared<Bullet>(owner->GetOwner(), ObjectType::EBullet, 100.);
		auto bulletTransform = bullet->AddComponent<TransformComponent>();

		double rotate = angle + bulletNumber * PI / TOTAL_BULLET_NUMBER;

		math::vec2 startPositionGap = { START_POS_OFFSET * cos(rotate) + START_POS_OFFSET * sin(rotate), START_POS_OFFSET * cos(rotate) - START_POS_OFFSET * sin(rotate) };

		bulletTransform->SetPosition(startPos.x + startPositionGap.x, startPos.y + startPositionGap.y);
		bulletTransform->SetScale(20., 20.);
		bulletTransform->SetRotation(rotate);

		boss->GetOwner()->AddObject(ObjectType::EBullet, bullet);
		bulletNumber++;
	}
}

void Boss::BossAttack2::TestForExit(Object* owner)
{
	auto boss = dynamic_cast<Boss*>(owner);
	if (bulletNumber == TOTAL_BULLET_NUMBER)
	{
		boss->ChangeState(&boss->bossIdle);
	}
}
