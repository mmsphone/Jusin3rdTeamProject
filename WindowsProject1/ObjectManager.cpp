#include "pch.h"
#include "Object.h"
#include "ObjectManager.h"

void ObjectManager::Initialize()
{
	objects.resize(static_cast<int>(ObjectType::Count));
	deadObjects.resize(static_cast<int>(ObjectType::Count));
}

void ObjectManager::Update(double dt)
{
	constexpr int typeSize = static_cast<int>(ObjectType::Count);

	for (int type = 0; type < typeSize; type++)
	{
		for (auto iter = objects[type].begin(); iter != objects[type].end(); iter++)
		{
			iter->get()->Update(dt);
			if (iter->get()->IsDead())
			{
				deadObjects[type].push_back(iter);
			}
		}
	}
	CleanDeadObjects();
}

void ObjectManager::Render(HDC hdc)
{
	for (const auto& objList : objects)
	{
		for (const auto& obj : objList)
		{
			obj->Render(hdc);
		}
	}
}

void ObjectManager::Render(LPDIRECT3DDEVICE9 d3dDevice)
{
	for (const auto& objList : objects)
	{
		for (const auto& obj : objList)
		{
			obj->Render(d3dDevice);
		}
	}
}

void ObjectManager::AddObject(ObjectType type, std::shared_ptr<Object> object)
{
	objects[static_cast<int>(type)].push_back(object);
}

void ObjectManager::RemoveObject(ObjectType type, std::shared_ptr<Object> object)
{
	auto& objectList = objects[static_cast<int>(type)];
	objectList.remove(object);
}

std::shared_ptr<Object> ObjectManager::FindByTag(const std::string& tag) {
	for (auto& objList : objects) {
		for (auto& obj : objList) {
			if (obj->GetTag() == tag)
				return obj;
		}
	}
	return nullptr;
}

std::shared_ptr<Object> ObjectManager::GetFrontObject(ObjectType type)
{
	return objects[static_cast<int>(type)].empty() ? nullptr : objects[static_cast<int>(type)].front();
}

std::list<std::shared_ptr<Object>> ObjectManager::GetObjectList(ObjectType type)
{
	return objects[static_cast<int>(type)];
}

void ObjectManager::ClearObjectList(ObjectType type)
{
	int index = static_cast<int>(type);
	objects[index].clear();
	deadObjects[index].clear();
}

void ObjectManager::ClearAll()
{
	objects.clear();
}

void ObjectManager::CleanDeadObjects() {
	constexpr int typeSize = static_cast<int>(ObjectType::Count);
	
	for (int type = 0; type < typeSize; type++)
	{
		for (const auto& deadObj : deadObjects[type])
		{
			objects[type].erase(deadObj);
		}
		deadObjects[type].clear();
	}
}