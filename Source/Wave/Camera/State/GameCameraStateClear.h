// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameCameraState.h"

class WAVE_API GameCameraStateClear : public GameCameraState
{
public:
	GameCameraStateClear();
	~GameCameraStateClear();

	void OnStart(AGameCameraActor * Actor) override;
	void OnUpdate(AGameCameraActor * Actor) override;
};
