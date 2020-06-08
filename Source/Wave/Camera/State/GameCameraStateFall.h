// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameCameraState.h"

class WAVE_API GameCameraStateFall : public GameCameraState
{
public:
	GameCameraStateFall();
	GameCameraStateFall(const FVector & FallPoint) : FallPoint(FallPoint) {};
	~GameCameraStateFall();

	void OnStart(AGameCameraActor * Actor) override;
	void OnUpdate(AGameCameraActor * Actor) override;

	FVector FallPoint;
};
