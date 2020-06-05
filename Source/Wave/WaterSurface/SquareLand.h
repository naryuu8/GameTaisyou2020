// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../WaterSurface/LandPoint.h"
#include "SquareLand.generated.h"

class OBB2D
{
public:
	FVector Position;
	FVector HorizontalVector;
	FVector VerticalVector;

	FVector GetPosition() { return Position; }

	FVector GetVector(int directionNumber)
	{
		switch (directionNumber)
		{
		case 0:
			return HorizontalVector;
		case 1:
			return VerticalVector;
		}

		return FVector::ZeroVector;
	}

	float GetLength(int number) { return GetVector(number).Size(); }
	FVector GetDirecton(int number) 
	{ 
		FVector direction = GetVector(number); 
		direction.Normalize(); return direction;
	}
};
/**
 * 
 */
UCLASS()
class WAVE_API ASquareLand : public ALandPoint
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	virtual void DebugDraw() override;

	float GetXLength() { return XLength; }
	float GetYLength() { return YLength; }

	OBB2D GetOBB() { return Obb; }

	bool OnGround(const FVector & Pos) override;
	bool OnGround(const FVector & Pos, float CircleRadius) override;
	bool InGround(const FVector & Pos, float CircleRadius) override;
	FVector AdjustMoveInLand(const FVector & Pos, float CircleRadius) override;
	FVector AdjustMoveOutWater(const FVector & OldPos, FVector MovedPos, FVector & MoveVec, float CircleRadius) override;
	FVector AdjustMoveOutWater(const FVector & OldPos, FVector MovedPos, FVector & MoveVec, float XLen, float YLen) override;

protected:
	UPROPERTY(EditAnywhere)
		float XLength = 100.0f;
	UPROPERTY(EditAnywhere)
		float YLength = 100.0f;

	OBB2D Obb;
};
