// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------


//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#pragma once
#include <vector>
#include "UIScreen.h"


///////////////////////////////////////////////////////////////////////////////
//HUD class
///////////////////////////////////////////////////////////////////////////////
class HUD : public UIScreen
{
public:
	//Enum State
	enum State {
		EMainMenu,
		EGameplay,
		EPaused,
		EItem,
		EQuit
	};

	//=========================================================================
	// public methods.
	//=========================================================================
	//コンストラクタ
	explicit HUD(class Game* game);

    //デストラクタ
	~HUD();

	//Update
	void Update(float deltaTime) override;


	//Getter,Setter
	int GetItemNum() const { return itemNum; }
	void SetItemNum(int num) { itemNum = num; }
	
	//Add,Remove
	void AddTargetComponent(class TargetComponent* tc);
	void RemoveTargetComponent(class TargetComponent* tc);


	void Draw(class Shader* shader) override;

	void DrawMainMenu(Shader* shader);
	void DrawGameplayHUD(Shader* shader);
	void DrawItemScreen(Shader* shader);
	void DrawBossMovie(Shader* shader);
	void DrawGameOverScreen(Shader* shader);
	void DrawGameClearScreen(Shader* shader);
	void DrawArrowOrBombCount(Shader* shader, int num, Vector2 pos);

protected:
	//=========================================================================
	// protected variables.
	//=========================================================================
	bool mTargetEnemy; //ターゲット
	int itemNum; //装備中のアイテム選択用変数
	int mStageNum;
	float mRadarRange; //レーダー範囲
	float mRadarRadius; //レーダー半径
	float mDrawTime; //Tips1
	float mBossDrawTime; //Tips3

	class Game* mGame; 
	class Font* mFont;
	class Texture* mHealthBar;
	class Texture* mHealthRedBar;
	class Texture* mRadar;
	class Texture* mCrosshair;
	class Texture* mCrosshairEnemy;
	class Texture* mBlipTex;
	class Texture* mRadarArrow;
	class Texture* mTitle;
	class Texture* mStaminaBar;
	class Texture* mStaminaFrame;
	class Texture* mHighlight;
	class Texture* mSword;
	class Texture* mBow;
	class Texture* mBomb;
	class Texture* mBow2;
	class Texture* mBomb2;
	class Texture* mSword2;
	class Texture* mTorch2;
	class Texture* mTorch;
	class Texture* mCross;
	class Texture* mFrame;
	class Texture* mFrame2;
	class Texture* mLine;
	class Texture* mTimer;
	class Texture* mStart;
	class Texture* mTips;
	class Texture* mTips2;
	class Texture* tex;
	class Texture* scoretex;
	class Texture* bosstex;
	std::vector<class TargetComponent*> mTargetComps;
	std::vector<Vector2> mBlips;


	//=========================================================================
	// protected methods.
	//=========================================================================
	void UpdateRadar(float deltaTime);

};
