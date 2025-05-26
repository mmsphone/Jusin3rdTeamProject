#pragma once
class Object;
class ObjectComponent {
public:
    Object* owner = nullptr;
public:
    virtual void Initialize() = 0;
    virtual void Update(double dt) = 0;
    virtual ~ObjectComponent() {}
};