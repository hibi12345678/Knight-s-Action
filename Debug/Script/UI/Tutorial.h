//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#pragma once
#include "Game.h"
#include "SoundEvent.h"
#include "UIScreen.h"


///////////////////////////////////////////////////////////////////////////////
//utorial class
///////////////////////////////////////////////////////////////////////////////
class Tutorial : public UIScreen
{
public:
	//=========================================================================
	// public methods.
	//=========================================================================
	//コンストラクタ
	Tutorial(class Game* game);

	//デストラクタ
	~Tutorial();

private:
	//=========================================================================
	// private variables.
	//=========================================================================
	SoundEvent mMusicEvent;
	
	class UIActor* uiActor;
};
