#include "ItemActorBase.h"
#include "Game.h"
#include "FollowActor.h"
#include "SkeletalMeshComponent.h"
#include "Math.h"
#include "MeshComponent.h"
#include "Mesh.h"
#include "Renderer.h"
ItemActorBase::ItemActorBase(Game* game, float scale, int num)
    : Actor(game)
    , mNum(num)

{
	SetScale(scale);
    mc = new MeshComponent(this);
}

void ItemActorBase::UpdateActor(float deltaTime)
{


    Actor::UpdateActor(deltaTime);
	Game* game = GetGame();
	if (mNum == 0) {
		Position = game->GetPlayer()->GetSekltalMesh()->GetBonePosition("Sword_joint");
		Rotation = game->GetPlayer()->GetSekltalMesh()->GetBoneRotation("Sword_joint");
	}

	else if (mNum == 1) {
		Position = game->GetPlayer()->GetSekltalMesh()->GetBonePosition("Shield_joint");
		Rotation = game->GetPlayer()->GetSekltalMesh()->GetBoneRotation("Shield_joint");
	}

	Quaternion playerRotation = game->GetPlayer()->GetRotation();
	Rotation = Quaternion::Concatenate(Rotation, playerRotation);

	SetRotation(Rotation);
	Vector3 playerPosition = game->GetPlayer()->GetPosition();
	Matrix4 playerTransform = Matrix4::CreateFromQuaternion(playerRotation); // ��]�s����擾
	Vector3 globalWeaponPos = Vector3::Transform(Position, playerTransform); // ���[�J�����W��ϊ�
	globalWeaponPos += playerPosition; // �v���C���[�̈ʒu�����Z
	SetPosition(globalWeaponPos);
}