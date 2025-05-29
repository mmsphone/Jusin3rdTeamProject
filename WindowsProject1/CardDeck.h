#pragma once
#include "Object.h"

class CardDeck : public Object {
public:
    CardDeck(ObjectManager* pOwner);

    void Render(HDC hdc) override;
};
