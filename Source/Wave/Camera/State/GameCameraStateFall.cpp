// Fill out your copyright notice in the Description page of Project Settings.

#include "GameCameraStateFall.h"
#include "../GameCameraActor.h"

GameCameraStateFall::GameCameraStateFall()
{
}

GameCameraStateFall::~GameCameraStateFall()
{
}

void GameCameraStateFall::OnStart(AGameCameraActor * Actor)
{
}

void GameCameraStateFall::OnUpdate(AGameCameraActor * Actor)
{
	Actor->SetActorLocation(FMath::Lerp<FVector>(Actor->GetActorLocation(), FallPoint, 0.1f));
	Actor->GameCameraBoom->SetVertical(75.0f, 0.05f);
	Actor->GameCameraBoom->SetTargetDistance(Actor->CharacterDistance_Far, 0.1f);
}
