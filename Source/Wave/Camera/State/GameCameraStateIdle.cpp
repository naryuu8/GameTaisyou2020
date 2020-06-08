#include "GameCameraStateIdle.h"
#include "../GameCameraActor.h"
#include "Kismet/GameplayStatics.h"
#include "GameCameraStateFocus.h"


GameCameraStateIdle::GameCameraStateIdle()
{
}

GameCameraStateIdle::~GameCameraStateIdle()
{
}

void GameCameraStateIdle::OnStart(AGameCameraActor * Actor)
{
}

void GameCameraStateIdle::OnUpdate(AGameCameraActor * Actor)
{
	if (Actor->FocusPoints.Num() > 0)
	{
		Actor->ChangeState(new GameCameraStateFocus());
		return;
	}

	Actor->InputChangeType();

	float FollowDist = 0.0f;
	FVector FollowPos;

	switch (Actor->Type)
	{
	case AGameCameraActor::FollowType::FieldCenter:
		FollowDist = Actor->FieldDistance;
		FollowPos = Actor->FieldCenterPos;
		break;
	case AGameCameraActor::FollowType::CharacterFollow_Far:
		FollowDist = Actor->CharacterDistance_Far;
		if (Actor->FollowTarget) FollowPos = Actor->FollowTarget->GetActorLocation();
		break;
	case AGameCameraActor::FollowType::CharacterFollow_Near:
		FollowDist = Actor->CharacterDistance_Near;
		if (Actor->FollowTarget) FollowPos = Actor->FollowTarget->GetActorLocation();
		break;
	}

	// なめらかに追従
	Actor->SetActorLocation(FMath::Lerp<FVector>(Actor->GetActorLocation(), FollowPos, 0.1f));
	// カメラの距離をセット
	Actor->GameCameraBoom->SetTargetDistance(FollowDist);
}
