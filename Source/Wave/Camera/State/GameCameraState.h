// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class AGameCameraActor;

class WAVE_API GameCameraState
{
public:
	GameCameraState();
	virtual ~GameCameraState();

	virtual void OnStart(AGameCameraActor * Actor) = 0;
	virtual void OnUpdate(AGameCameraActor * Actor) = 0;
};
