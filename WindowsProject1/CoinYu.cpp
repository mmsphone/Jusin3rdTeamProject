#include "pch.h"
#include "CoinYu.h"
#include "TransformComponent.h"

CoinYu::CoinYu(ObjectManager* owner, ObjectType objType, float _fX, float _fY)
	:Object(owner, objType, RenderType::Ellipse)
{
	auto transform = AddComponent<TransformComponent>();
	transform->SetPosition(_fX, _fY);
	transform->SetScale(80.0f, 80.0f);
}
