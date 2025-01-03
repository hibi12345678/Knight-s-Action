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
#include "HUD.h"
#include <algorithm>
#include <iostream>
#include "BossActor.h"
#include "FollowActor.h"
#include "Font.h"
#include "Game.h"
#include "GameTimer.h"
#include "GBuffer.h"
#include "PhysWorld.h"
#include "Renderer.h"
#include "Shader.h"
#include "TargetComponent.h"
#include "Texture.h"


///////////////////////////////////////////////////////////////////////////////
// HUD class
///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
//      コンストラクタです.
//-----------------------------------------------------------------------------
HUD::HUD(Game* game)
	:UIScreen(game)
	, mRadarRange(2000.0f)
	, mRadarRadius(92.0f)
	, mTargetEnemy(false)
	, mGame(game)
	, mDrawTime(0.0f)
	, mBossDrawTime(0.0f)
	, itemNum(0)
{
	Renderer* r = mGame->GetRenderer();
	mRadar = r->GetTexture("Assets/Texture/Radar.png");
	mCrosshair = r->GetTexture("Assets/Texture/CrosshairOrig2.png");
	mBlipTex = r->GetTexture("Assets/Texture/Blip.png");
	mRadarArrow = r->GetTexture("Assets/Texture/RadarArrow.png");
	mTitle = r->GetTexture("Assets/Texture/Title.png");
	mStaminaBar= r->GetTexture("Assets/Texture/Stamina.png");
	mStaminaFrame = r->GetTexture("Assets/Texture/StaminaFrame.png");
	mHealthBar= r->GetTexture("Assets/Texture/Health.png");
	mHealthRedBar = r->GetTexture("Assets/Texture/HealthRed.png");
	mFont = mGame->GetFont("Assets/Carlito-Regular.ttf");
	mHighlight = mGame->GetRenderer()->GetTexture("Assets/Texture/ItemButtonOrange.png");
	mSword = mGame->GetRenderer()->GetTexture("Assets/Texture/SwordTex.png");
	mTorch = mGame->GetRenderer()->GetTexture("Assets/Texture/TorchTex.png");
	mBow = mGame->GetRenderer()->GetTexture("Assets/Texture/BowTex.png");
	mBomb = mGame->GetRenderer()->GetTexture("Assets/Texture/BombTex.png");
	mSword2 = mGame->GetRenderer()->GetTexture("Assets/Texture/SwordTex2.png");
	mTorch2 = mGame->GetRenderer()->GetTexture("Assets/Texture/TorchTex2.png");
	mBow2 = mGame->GetRenderer()->GetTexture("Assets/Texture/BowTex2.png");
	mBomb2 = mGame->GetRenderer()->GetTexture("Assets/Texture/BombTex2.png");
	mCross = mGame->GetRenderer()->GetTexture("Assets/Texture/×.png");
	mFrame = mGame->GetRenderer()->GetTexture("Assets/Texture/Frame.png");
	mFrame2 = mGame->GetRenderer()->GetTexture("Assets/Texture/Frame2.png");
	mLine = mGame->GetRenderer()->GetTexture("Assets/Texture/Line.png");
	mTimer = mGame->GetRenderer()->GetTexture("Assets/Texture/Time.png");
	mStart = mGame->GetRenderer()->GetTexture("Assets/Texture/Start.png");
	mTips = mGame->GetRenderer()->GetTexture("Assets/Texture/Tips3.png");
	mTips2 = mGame->GetRenderer()->GetTexture("Assets/Texture/Tips4.png");
}


//-----------------------------------------------------------------------------
//      デストラクタです.
//-----------------------------------------------------------------------------
HUD::~HUD()
{
}


//-----------------------------------------------------------------------------
//  Update
//-----------------------------------------------------------------------------
void HUD::Update(float deltaTime)
{
	mStageNum = mGame->GetStageNumber();
	UIScreen::Update(deltaTime);
	UpdateRadar(deltaTime);
	if (Game::GameState::EGameplay == mGame->GetState()) {
		if (mStageNum == 1) {
			mDrawTime += deltaTime;
		}
		else if (mStageNum == 3) {
			mBossDrawTime += deltaTime;
		}
	}
}



void HUD::Draw(Shader* shader) {
	switch (mGame->GetState()) {
	case Game::GameState::EMainMenu:
		DrawMainMenu(shader);
		break;
	case Game::GameState::EGameplay:
		DrawGameplayHUD(shader);
		break;
	case Game::GameState::EItem:
		DrawItemScreen(shader);
		break;
	case Game::GameState::EBossMovie:
		DrawBossMovie(shader);
		break;
	case Game::GameState::EGameOver:
		DrawGameOverScreen(shader);
		break;
	case Game::GameState::EGameClear:
		DrawGameClearScreen(shader);
		break;
	default:
		break;
	}
}


void HUD::DrawMainMenu(Shader* shader) {
	DrawTexture(shader, mStart, Vector2(0.0f, 0.0f), 1.1f);
}


void HUD::DrawGameplayHUD(Shader* shader) {
	const Vector2 cRadarPos(-413.0f, 283.0f);
	DrawTexture(shader, mRadar, cRadarPos, 1.0f);

	// Blipsの描画
	for (const Vector2& blip : mBlips) {
		DrawTexture(shader, mBlipTex, cRadarPos + blip, 1.0f);
	}

	// レーダーの矢印描画
	DrawTexture(shader, mRadarArrow, cRadarPos);

	// プレイヤーの取得と関連描画
	FollowActor* followActor = mGame->GetPlayer();
	if (followActor != nullptr) {
		// スタミナバーの描画
		float stamina = followActor->GetStamina();
		DrawTexture(shader, mStaminaBar, Vector2(-276.0f, 320.0f), stamina * 0.99f, false, 1);
		DrawTexture(shader, mStaminaFrame, Vector2(-280.0f, 320.0f), 1.0f, false, 1);

		// ヘルスバーの描画
		float health = followActor->GetHealth();
		float healthRed = followActor->GetHealthRed();
		DrawTexture(shader, mHealthRedBar, Vector2(-276.0f, 345.0f), healthRed * 0.99f, true, 1);
		DrawTexture(shader, mHealthBar, Vector2(-276.0f, 345.0f), health * 0.99f, true, 1);
		DrawTexture(shader, mStaminaFrame, Vector2(-280.0f, 345.0f), 1.0f, true, 1);

		// プレイヤーアイテムの描画
		Vector2 itemPos(447.0f, -319.0f);
		DrawTexture(shader, mRadar, itemPos, 0.65f);
		switch (followActor->GetItemState()) {
		case FollowActor::ItemState::ESword:
			DrawTexture(shader, mSword2, itemPos + Vector2(0.0f, -5.0f), 1.0f);
			break;
		case FollowActor::ItemState::ETorch:
			DrawTexture(shader, mTorch2, itemPos, 1.0f);
			break;
		case FollowActor::ItemState::EBow:
			DrawTexture(shader, mBow2, itemPos + Vector2(-5.0f, 5.0f), 1.0f);
			DrawTexture(shader, mCrosshair, Vector2::Zero, 0.5f);
			DrawTexture(shader, mCross, Vector2(467.0f, -349.0f), 1.0f);
			DrawArrowOrBombCount(shader, followActor->GetArrowNum(), Vector2(487.0f, -351.0f));
			break;
		case FollowActor::ItemState::EBomb:
			DrawTexture(shader, mBomb2, itemPos + Vector2(-8.0f, 5.0f), 1.0f);
			DrawTexture(shader, mCross, Vector2(467.0f, -349.0f), 1.0f);
			DrawArrowOrBombCount(shader, followActor->GetBombNum(), Vector2(487.0f, -351.0f));
			break;
		default:
			break;
		}

		int mTime = mGame->GetTimer()->GetElapsedTime();
		// 分と秒に変換
		int minutes = mTime / 60;
		int seconds = mTime % 60;
		if (minutes < 10) {
			std::string str = std::to_string(minutes);
			tex = mFont->RenderText(str, Color::White, 24, 1);
			DrawTexture(shader, tex, Vector2(460.0, 334.0f));
		}
		else {
			std::string str = std::to_string(minutes);
			tex = mFont->RenderText(str, Color::White, 24, 1);
			DrawTexture(shader, tex, Vector2(455.0, 334.0f));
		}
		if (seconds < 10) {
			std::string str = std::to_string(seconds);
			tex = mFont->RenderText(str, Color::White, 24, 1);
			DrawTexture(shader, tex, Vector2(495.0, 334.0f));
			tex = mFont->RenderText("0", Color::White, 24, 1);
			DrawTexture(shader, tex, Vector2(483.0, 334.0f));
		}
		else {
			std::string str = std::to_string(seconds);
			tex = mFont->RenderText(str, Color::White, 24, 1);
			DrawTexture(shader, tex, Vector2(490.0, 334.0f));
		}

		tex = mFont->RenderText(":", Color::White, 26, 1);
		DrawTexture(shader, tex, Vector2(473.0, 336.0f));
		DrawTexture(shader, mTimer, Vector2(425.0, 337.0f), 0.25f);

		if (mStageNum == 1 || mStageNum == 2) {

			if (mDrawTime < 0.5f) {
				DrawTexture(shader, mTips, Vector2(450.0f - mDrawTime * 120.0f, 170.0f), 0.07);
			}

			else if (mDrawTime >= 0.5f) {
				DrawTexture(shader, mTips, Vector2(390.0f, 170.0f), 0.07);
				tex = mFont->RenderText("- Go Lights!", Color::Black, 22, 1);
				DrawTexture(shader, tex, Vector2(345.0f, 190.0f));
				tex = mFont->RenderText("- Enemy Drops Items", Color::Black, 22, 1);
				DrawTexture(shader, tex, Vector2(390.0f, 150.0f));
			}


		}

		else if (mStageNum == 3) {

			if (mBossDrawTime < 0.5f) {
				DrawTexture(shader, mTips, Vector2(450.0f - mDrawTime * 120.0f, 170.0f), 0.07);
			}

			else if (mBossDrawTime >= 0.5f) {
				DrawTexture(shader, mTips, Vector2(390.0f, 170.0f), 0.07);
				tex = mFont->RenderText("- Head is weakness", Color::Black, 22, 1);
				DrawTexture(shader, tex, Vector2(390.0f, 190.0f));
				tex = mFont->RenderText("- Bombs are effective", Color::Black, 22, 1);
				DrawTexture(shader, tex, Vector2(395.0f, 150.0f));
			}
		}
	}

	// ボスの描画
	for (auto boss : mGame->GetBoss()) {
		if (boss != nullptr) {
			bosstex = mFont->RenderText("BossName", Color::White, 24);
			DrawTexture(shader, bosstex, Vector2(-200.0, -265.0f));
			float health = boss->GetHealth();
			DrawTexture(shader, mHealthBar, Vector2(-250.0f, -300.0f), health * 1.5, false, 1);
			DrawTexture(shader, mStaminaFrame, Vector2(-250.0f, -300.0f), 1.5, false, 1);
		}
	}
}


void HUD::DrawItemScreen(Shader* shader) 
{
	DrawTexture(shader, mFrame, Vector2(0.0f, -160.0f), 0.7f);
	DrawTexture(shader, mFrame2, Vector2(0.0f, 120.0f), 0.7f);
	DrawTexture(shader, mHighlight, Vector2(-270.0f, -160.0f), 0.7f);
	if (itemNum == 0) {
		DrawTexture(shader, mSword, Vector2(-270.0f, -160.0f), 0.7f);

	}
	else if (itemNum == 1) {
		DrawTexture(shader, mTorch, Vector2(-270.0f, -160.0f), 0.7f);

	}
	else if (itemNum == 2) {
		DrawTexture(shader, mBow, Vector2(-270.0f, -160.0f), 0.7f);


	}
	else if (itemNum == 3) {
		DrawTexture(shader, mBomb, Vector2(-270.0f, -160.0f), 0.7f);

	}
}


void HUD::DrawBossMovie(Shader* shader) {
	DrawTexture(shader, mTips2, Vector2(0.0f, -250.0f), 1.00f);
	tex = mFont->RenderText("The Boss appears!", Color::Black, 48, 1);
	DrawTexture(shader, tex, Vector2(0.0f, -250.0f));
}

void HUD::DrawGameOverScreen(Shader* shader) {
	int mTime = mGame->GetTimer()->GetElapsedTime();
	// 分と秒に変換
	int minutes = mTime / 60;
	int seconds = mTime % 60;
	if (minutes < 10) {
		std::string str = std::to_string(minutes);
		tex = mFont->RenderText(str, Color::White, 44, 1);
		DrawTexture(shader, tex, Vector2(50.0, 200.0f));
	}
	else {
		std::string str = std::to_string(minutes);
		tex = mFont->RenderText(str, Color::White, 44, 1);
		DrawTexture(shader, tex, Vector2(35.0, 200.0f));
	}
	if (seconds < 10) {
		std::string str = std::to_string(seconds);
		tex = mFont->RenderText(str, Color::White, 44, 1);
		DrawTexture(shader, tex, Vector2(120.0, 200.0f));
		tex = mFont->RenderText("0", Color::White, 44, 1);
		DrawTexture(shader, tex, Vector2(95.0, 200.0f));
	}
	else {
		std::string str = std::to_string(seconds);
		tex = mFont->RenderText(str, Color::White, 44, 1);
		DrawTexture(shader, tex, Vector2(105.0, 200.0f));
	}

	tex = mFont->RenderText(":", Color::White, 44, 1);
	DrawTexture(shader, tex, Vector2(73.0, 203.0f));
	scoretex = mFont->RenderText("Time : ", Color::White, 44, 1);
	DrawTexture(shader, scoretex, Vector2(-70.0f, 200.0f));
	DrawTexture(shader, mLine, Vector2(0.0f, 170.0f), 2.0f);
}

void HUD::DrawGameClearScreen(Shader* shader) {
	DrawTexture(shader, mTips2, Vector2(0.0f, 200.0f), 1.5f);
	int mTime = mGame->GetTimer()->GetElapsedTime();
	// 分と秒に変換
	int minutes = mTime / 60;
	int seconds = mTime % 60;
	if (minutes < 10) {
		std::string str = std::to_string(minutes);
		tex = mFont->RenderText(str, Color::White, 44, 1);
		DrawTexture(shader, tex, Vector2(70.0, 180.0f));
	}
	else {
		std::string str = std::to_string(minutes);
		tex = mFont->RenderText(str, Color::White, 44, 1);
		DrawTexture(shader, tex, Vector2(55.0, 180.0f));
	}
	if (seconds < 10) {
		std::string str = std::to_string(seconds);
		tex = mFont->RenderText(str, Color::White, 44, 1);
		DrawTexture(shader, tex, Vector2(140.0, 180.0f));
		tex = mFont->RenderText("0", Color::White, 44, 1);
		DrawTexture(shader, tex, Vector2(115.0, 180.0f));
	}
	else {
		std::string str = std::to_string(seconds);
		tex = mFont->RenderText(str, Color::White, 44, 1);
		DrawTexture(shader, tex, Vector2(125.0, 180.0f));
	}

	tex = mFont->RenderText(":", Color::White, 44, 1);
	DrawTexture(shader, tex, Vector2(93.0, 182.0f));

	scoretex = mFont->RenderText("Clear Time :", Color::White, 44, 1);
	DrawTexture(shader, scoretex, Vector2(-120.0f, 180.0f));
	DrawTexture(shader, mLine, Vector2(0.0f, 160.0f), 3.0f);
}

void HUD::DrawArrowOrBombCount(Shader* shader, int num, Vector2 pos) {
	
	std::string str = std::to_string(num);
	tex = mFont->RenderText(str, Color::White, 24, 1);
	DrawTexture(shader, tex, pos);
}


//-----------------------------------------------------------------------------
//  TargetComponent vectorに追加
//-----------------------------------------------------------------------------
void HUD::AddTargetComponent(TargetComponent* tc)
{
	mTargetComps.emplace_back(tc);

}


//-----------------------------------------------------------------------------
//  TargetComponent vectorから削除
//-----------------------------------------------------------------------------
void HUD::RemoveTargetComponent(TargetComponent* tc)
{
	auto iter = std::find(mTargetComps.begin(), mTargetComps.end(),
		tc);
	mTargetComps.erase(iter);
}


//-----------------------------------------------------------------------------
//  Raderの描画処理
//-----------------------------------------------------------------------------
void HUD::UpdateRadar(float deltaTime)
{
	
	if (Game::GameState::EGameplay == mGame->GetState()) {

		mBlips.clear();

		Vector3 playerPos = mGame->GetPlayer()->GetPosition();
		Vector2 playerPos2D(playerPos.y, playerPos.x);

		Vector3 playerForward = mGame->GetPlayer()->GetForward();
		Vector2 playerForward2D(playerForward.x, playerForward.y);

		float angle = Math::Atan2(playerForward2D.y, playerForward2D.x);
		Matrix3 rotMat = Matrix3::CreateRotation(angle);
		
		for (auto tc : mTargetComps)
		{

			Vector3 targetPos = tc->GetOwner()->GetPosition();
			Vector2 actorPos2D(targetPos.y, targetPos.x);
			Vector2 playerToTarget = actorPos2D - playerPos2D;

			if (playerToTarget.LengthSq() <= (mRadarRange * mRadarRange))
			{
				Vector2 blipPos = playerToTarget;
				blipPos *= mRadarRadius / mRadarRange;

				blipPos = Vector2::Transform(blipPos, rotMat);
				mBlips.emplace_back(blipPos);
			}
		}
	}

}
