// Fill out your copyright notice in the Description page of Project Settings.

#include "GameCameraStateClear.h"
#include "../GameCameraActor.h"

GameCameraStateClear::GameCameraStateClear()
{
}

GameCameraStateClear::~GameCameraStateClear()
{
}

void GameCameraStateClear::OnStart(AGameCameraActor * Actor)
{
}

void GameCameraStateClear::OnUpdate(AGameCameraActor * Actor)
{
	FVector Direction = Actor->FollowTarget->GetActorForwardVector().GetSafeNormal2D();
	Actor->SetActorLocation(FMath::Lerp<FVector>(Actor->GetActorLocation(), Actor->FollowTarget->GetActorLocation(), 0.1f));
	Actor->GameCameraBoom->SetVertical(10.0f, 0.1f);
	Actor->GameCameraBoom->SetHorizontal(-Direction, 0.1f);
	Actor->GameCameraBoom->SetTargetDistance(Actor->CharacterDistance_Near, 0.1f);
}
