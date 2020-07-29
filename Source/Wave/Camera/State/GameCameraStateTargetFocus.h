// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameCameraState.h"


class WAVE_API GameCameraStateTargetFocus : public GameCameraState
{
public:
	GameCameraStateTargetFocus() {};
	GameCameraStateTargetFocus(FVector FocusPoint) : FocusPoint(FocusPoint) {};
	~GameCameraStateTargetFocus() {};

	void OnStart(AGameCameraActor * Actor) override;
	void OnUpdate(AGameCameraActor * Actor) override;

private:
	FVector FocusPoint;
};
