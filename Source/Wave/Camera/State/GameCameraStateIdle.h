// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameCameraState.h"

class WAVE_API GameCameraStateIdle : public GameCameraState
{
public:
	GameCameraStateIdle();
	~GameCameraStateIdle();

	void OnStart(AGameCameraActor * Actor) override;
	void OnUpdate(AGameCameraActor * Actor) override;
};
