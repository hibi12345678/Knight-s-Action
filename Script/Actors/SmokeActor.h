#include "EffectActor.h"

class SmokeActor : public Actor
{
public:
    SmokeActor(Game* game);
        
    void UpdateActor(float deltaTime) override;


private:
    float mLifeSpan;
};