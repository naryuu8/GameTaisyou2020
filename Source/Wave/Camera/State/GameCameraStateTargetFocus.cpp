// Fill out your copyright notice in the Description page of Project Settings.

#include "GameCameraStateTargetFocus.h"
#include "../GameCameraActor.h"

void GameCameraStateTargetFocus::OnStart(AGameCameraActor * Actor)
{
}

void GameCameraStateTargetFocus::OnUpdate(AGameCameraActor * Actor)
{
	// なめらかに追従
	Actor->SetActorLocation(FMath::Lerp<FVector>(Actor->GetActorLocation(), FocusPoint, 0.1f));
	// カメラの距離をセット
	Actor->GameCameraBoom->SetTargetDistance(Actor->CharacterDistance_Far);

	Actor->GameCameraBoom->InputUpdateRotation();
}
