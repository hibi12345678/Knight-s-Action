//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#pragma once
#include "ItemActorBase.h"


///////////////////////////////////////////////////////////////////////////////
//TorchItemActor class
///////////////////////////////////////////////////////////////////////////////
class TorchItemActor : public ItemActorBase
{
public:
	//=========================================================================
	// public methods.
	//=========================================================================
    //コンストラクタ
	TorchItemActor(class Game* game,float scale, int num);

	//Update
	void UpdateActor(float deltaTime) override;

private:
	//=========================================================================
	// private variables.
	//=========================================================================
	std::vector<class PointLightComponent*> mPointLights;
};