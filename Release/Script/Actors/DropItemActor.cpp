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
#include "DropItemActor.h"
#include <cmath>
#include <cstdlib>
#include <ctime>
#include "AudioComponent.h"
#include "BoxComponent.h"
#include "FollowActor.h"
#include "Game.h"
#include "Mesh.h"
#include "MeshComponent.h"
#include "Random.h"
#include "Renderer.h"


///////////////////////////////////////////////////////////////////////////////
// App class
///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
//      コンストラクタです.
//-----------------------------------------------------------------------------
DropItemActor::DropItemActor(Game* game)
	:Actor(game)
	, mLifeSpan(10.0f)
	, blinkTime(0.0f)
	, blinkInterval(0.2f)
	, isVisible(true)
	, randomValue(0)
{
	
	mc = new MeshComponent(this);
	game->AddDropItem(this);
	randomValue = rand() % 2;
	if (randomValue ==0) {
		SetScale(30.0f);
		AABB myBox(Vector3(-1.0f, -1.0f, 0.0f),
			Vector3(1.0f, 1.0f, 1.0f));
		Mesh* mesh = GetGame()->GetRenderer()->GetMesh("Assets/Object/ArrowDot.gpmesh");
		mBox = new BoxComponent(this);
		mBox->SetObjectBox(myBox);
		mBox->SetShouldRotate(false);
		mc->SetMesh(mesh);
		itemValue = 0;
	}
	else if (randomValue == 1) {
		SetScale(0.2f);
		AABB myBox(Vector3(-150.0f, -150.0f, 0.0f),
			Vector3(150.0f, 150.0f, 300.0f));
		Mesh* mesh = GetGame()->GetRenderer()->GetMesh("Assets/Object/Bomb.gpmesh");
		mBox = new BoxComponent(this);
		mBox->SetObjectBox(myBox);
		mBox->SetShouldRotate(false);
		mc->SetMesh(mesh);
		itemValue = 1;
	}
}


//-----------------------------------------------------------------------------
//      デストラクタです.
//-----------------------------------------------------------------------------
DropItemActor::~DropItemActor() {
	GetGame()->RemoveDropItem(this);
}


//-----------------------------------------------------------------------------
//  Update
//-----------------------------------------------------------------------------
void DropItemActor::UpdateActor(float deltaTime)
{
	Actor::UpdateActor(deltaTime);
	FixCollisions();
	mLifeSpan -= deltaTime;
	if (mLifeSpan < 3.0f)
	{
		// 経過時間を増加させる
		blinkTime += deltaTime;

		// 一定間隔を超えたら可視状態を切り替え
		if (blinkTime >= blinkInterval)
		{
			// 可視状態を反転させる
			isVisible = !isVisible;
			mc->SetVisible(isVisible);

			// タイマーをリセット
			blinkTime = 0.0f;
		}

		//消滅
		if (mLifeSpan < 0.0f)
		{
			SetState(EDead);
		}
	}

	float angle = 90.0f * deltaTime ;

	// Y軸回転を行うクォータニオンを生成
	Quaternion rotation = Quaternion::CreateFromAxisAngle(Math::ToRadians(angle));
	Quaternion mRotation = GetRotation();
	// 現在の回転に新しい回転を掛け合わせて回転させる
	mRotation = Quaternion::Concatenate(mRotation, rotation);
	// 回転をセット
	SetRotation(mRotation);
}


//-----------------------------------------------------------------------------
//  衝突処理
//-----------------------------------------------------------------------------
void DropItemActor::FixCollisions() {

	ComputeWorldTransform();

	const AABB& itemBox = mBox->GetWorldBox();
	Vector3 pos = GetPosition();
	auto* player = GetGame()->GetPlayer();
	if (player != nullptr) {
		const AABB& enemyBox = player->GetBox()->GetWorldBox();
		if (Intersect(itemBox, enemyBox))
		{
			SetState(EDead);
		}
	}
}
