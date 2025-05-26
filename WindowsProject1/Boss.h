#pragma once
#include "Object.h"
class Boss : public Object
{
    double speed;
public:
    Boss(ObjectManager*, ObjectType, double);
    //virtual void Render(HDC hdc);
    virtual void Update(double dt);
private:
    class BossIdle : public State {
        void Enter(Object*) override;
        void Update(Object*, double) override;
        void TestForExit(Object*) override;
        static constexpr double ATTACK_COOLDOWN = 2.;
        double timer = 0.;
    };

    class BossAttack1 : public State {
        void Enter(Object*) override;
        void Update(Object*, double) override;
        void TestForExit(Object*) override;
        static constexpr int TOTAL_BULLET_NUMBER = 50;
        int bulletNumber = 0;
        static constexpr double BULLET_COOLDOWN = 0.02;
        double timer = 0.;
        math::vec2 startPos;
        static constexpr double START_POS_OFFSET = 5;
        double startAngle;
    };

    class BossAttack2 : public State {
        void Enter(Object*) override;
        void Update(Object*, double) override;
        void TestForExit(Object*) override;
        static constexpr int TOTAL_BULLET_NUMBER = 12;
        int bulletNumber = 0;
        static constexpr double BULLET_COOLDOWN = 0.02;
        double timer = 0.;
        math::vec2 startPos;
        static constexpr double START_POS_OFFSET = 5;

        double angle = 0.;

    };

private:
    BossIdle bossIdle;
    BossAttack1 bossAttack1;
    BossAttack2 bossAttack2;


    math::vec2 area[4];

    bool clockwise = true;
};
