#include "GameCameraStateFocus.h"
#include "../GameCameraActor.h"
#include "GameCameraStateIdle.h"
#include "../../MyFunc.h"


GameCameraStateFocus::GameCameraStateFocus()
{
}

GameCameraStateFocus::~GameCameraStateFocus()
{
}

void GameCameraStateFocus::OnStart(AGameCameraActor * Actor)
{
}

void GameCameraStateFocus::OnUpdate(AGameCameraActor * Actor)
{
	if (Actor->FocusPoints.Num() == 0)
	{
		Actor->ChangeState(new GameCameraStateIdle());
		return;
	}

	FVector CameraCenterPos = Actor->GetActorLocation();

	float FollowDist = 0.0f;
	FVector CenterPos;
	FVector MaxDistPos;
	float MaxDist = 0.0f;

	FVector TargetPos = Actor->FollowTarget->GetActorLocation();
	CenterPos = TargetPos;
	for (AActor * Point : Actor->FocusPoints)
		CenterPos += Point->GetActorLocation();
	CenterPos /= Actor->FocusPoints.Num() + 1;

	auto CheckDistance = [&MaxDist, &MaxDistPos, Actor, CenterPos](const FVector & Pos)
	{
		//float Dist = FMath::Abs(FVector::DotProduct(Actor->GameCameraBoom->GetRightVector(), Pos - CenterPos));
		float Dist = FVector::Distance(CenterPos, Pos);
		if (Dist > MaxDist)
		{
			MaxDist = Dist;
			MaxDistPos = Pos;
		}
	};

	float TargetDist = FVector::Distance(CenterPos, TargetPos);
	MaxDist = TargetDist;
	MaxDistPos = TargetPos;
	for (AActor * Point : Actor->FocusPoints)
	{
		CheckDistance(Point->GetActorLocation());
	}

	FollowDist = MaxDist + TargetDist + Actor->CharacterDistance_Far;

	if (Actor->GameCameraBoom->GetVerticalDegree() < 30.0f)
	{
		Actor->GameCameraBoom->SetVertical(30.0f, 0.1f);
	}
	//Actor->GameCameraBoom->SetHorizontal(-Actor->FollowTarget->GetActorForwardVector().GetSafeNormal2D(), 0.1f);

	// なめらかに追従
	Actor->SetActorLocation(FMath::Lerp<FVector>(Actor->GetActorLocation(), CenterPos, 0.05f));
	// カメラの距離をセット
	Actor->GameCameraBoom->SetTargetDistance(FollowDist, 0.05f);

	Actor->GameCameraBoom->InputUpdateRotation();
}
