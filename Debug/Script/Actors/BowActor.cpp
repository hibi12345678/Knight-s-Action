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
#include "BowActor.h"
#include "FollowActor.h"
#include "Game.h"
#include "Maths.h"
#include "Mesh.h"
#include "MeshComponent.h"
#include "Renderer.h"
#include "SkeletalMeshComponent.h"


///////////////////////////////////////////////////////////////////////////////
// BowActor class
///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
//      コンストラクタです.
//-----------------------------------------------------------------------------
BowActor::BowActor(Game* game, float scale, int num)
	: ItemActorBase(game, scale, num)
{	
	Mesh* mesh = GetGame()->GetRenderer()->GetMesh("Assets/Object/Bow.gpmesh");
	mc->SetMesh(mesh);
	
}


//-----------------------------------------------------------------------------
// Update
//-----------------------------------------------------------------------------
void BowActor::UpdateActor(float deltaTime)
{
	// 基底クラスのUpdateActor呼び出し
	ItemActorBase::UpdateActor(deltaTime);
	Game* game = GetGame();

	if (FollowActor::ItemState::EBow != game->GetPlayer()->GetItemState()) {
		SetState(Actor::EDead);
	}
}

