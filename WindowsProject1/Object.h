#pragma once
#include "ObjectComponent.h"

class ObjectManager;
class Object
{
protected:
    ObjectType objType;
    ObjectManager* owner;
    bool isDead;
    RenderType type;
    std::string tag = "none";
public:
    Object(ObjectManager*, ObjectType, RenderType type = RenderType::Rect);
    virtual void Update(double dt);
    virtual void Render(HDC hdc);
    virtual void Render(LPDIRECT3DDEVICE9 hdc) {};

    void SetDead() { isDead = true; };
    bool IsDead() const { return isDead; }
    ObjectManager* GetOwner() { return owner; };
    void SetTag(const std::string& s) { tag = s; }
    const std::string& GetTag() const { return tag; }
public:
    template<typename T, typename... Args>
    T* AddComponent(Args&&... args) {
        auto comp = std::make_unique<T>(std::forward<Args>(args)...);
        comp->owner = this;
        T* raw = comp.get();
        components.push_back(std::move(comp));
        componentMap[typeid(T)] = raw;
        return raw;
    }

    template<typename T>
    T* GetComponent() const {
        auto it = componentMap.find(typeid(T));
        if (it != componentMap.end()) return static_cast<T*>(it->second);
        return nullptr;
    }
private:
    std::vector<std::unique_ptr<ObjectComponent>> components;
    std::unordered_map<std::type_index, ObjectComponent*> componentMap;

protected: //GameObject State
    class State {
    public:
        virtual void Enter(Object* object) = 0;
        virtual void Update(Object* object, double dt) = 0;
        virtual void TestForExit(Object* object) = 0;
    protected:
        bool stateSwitch = false;
    };
    class State_Nothing : public State {
    public:
        void Enter(Object*) override {};
        void Update(Object*, double) override {};
        void TestForExit(Object*) override {};
    };
    State_Nothing stateNothing;
    State* currState;
public:
    void ChangeState(State* newState);
};
