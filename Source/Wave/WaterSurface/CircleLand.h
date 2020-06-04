// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../WaterSurface/LandPoint.h"
#include "CircleLand.generated.h"

UCLASS()
class WAVE_API ACircleLand : public ALandPoint
{
	GENERATED_BODY()

public:
	virtual void DebugDraw() override;

	float GetRadius() { return Radius; }

	bool OnGround(const FVector & Pos) override;
	bool OnGround(const FVector & Pos, float CircleRadius) override;
	bool InGround(const FVector & Pos, float CircleRadius) override;
	FVector AdjustMoveInLand(const FVector & Pos, float CircleRadius) override;
	FVector AdjustMoveOutWater(const FVector & OldPos, FVector MovedPos, FVector & MoveVec, float CircleRadius) override;
	FVector AdjustMoveOutWater(const FVector & OldPos, FVector MovedPos, FVector & MoveVec, float XLen, float YLen) override;

private:
	UPROPERTY(EditAnywhere)
		float Radius = 100.0f;
};
