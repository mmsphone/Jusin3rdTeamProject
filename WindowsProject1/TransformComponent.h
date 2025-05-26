#pragma once
#include "ObjectComponent.h"
class TransformComponent : public ObjectComponent {
public:
    math::vec2 position = { 0.0f, 0.0f }; 
    float rotation = 0.0f;            
    math::vec2 scale = { 1.0f, 1.0f }; 

public:
    TransformComponent() = default;
    virtual ~TransformComponent() override = default;
    virtual void Initialize() {};
    virtual void Update(double dt) override {};

    void SetPosition(float x, float y) {
        position.x = x;
        position.y = y;
    }

    void SetPosition(math::vec2 pos) {
        position = pos;
    }

    void Translate(float dx, float dy) {
        position.x += dx;
        position.y += dy;
    }

    void Translate(math::vec2 dpos) {
        Translate(dpos.x, dpos.y);
    }

    void SetRotation(float angle) {
        rotation = angle;
    }

    void Rotate(float dAngle) {
        rotation += dAngle;
    }

    void SetScale(float sx, float sy) {
        scale.x = sx;
        scale.y = sy;
    }

    void SetScale(math::vec2 s) {
        scale = s;
    }

    void ScaleBy(float sx, float sy) {
        scale.x *= sx;
        scale.y *= sy;
    }

    void ScaleBy(math::vec2 s) {
        ScaleBy(s.x, s.y);
    }

    math::vec2 GetPosition() const {
        return position;
    }

    float GetRotation() const {
        return rotation;
    }

    math::vec2 GetScale() const {
        return scale;
    }
};