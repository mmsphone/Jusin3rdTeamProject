#pragma once

class Object;
class ObjectManager
{
public:
	void Initialize();
	void Update(double dt);
	void Render(HDC);
	void Render(LPDIRECT3DDEVICE9);

	void AddObject(ObjectType type, std::shared_ptr<Object> object);
	void RemoveObject(ObjectType type, std::shared_ptr<Object> object);
	std::shared_ptr<Object> FindByTag(const std::string& tag);

	std::shared_ptr<Object> GetFrontObject(ObjectType type);
	std::list<std::shared_ptr<Object>> GetObjectList(ObjectType type);
	void ClearObjectList(ObjectType type);
	void ClearAll();
private:
	void CleanDeadObjects();
private:
	std::vector<std::list<std::shared_ptr<Object>>> objects;
	std::vector<std::vector<std::list<std::shared_ptr<Object>>::iterator>> deadObjects;
};
