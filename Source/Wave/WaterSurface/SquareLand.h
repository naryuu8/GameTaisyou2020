// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../WaterSurface/LandPoint.h"
#include "SquareLand.generated.h"

/**
 * 
 */
UCLASS()
class WAVE_API ASquareLand : public ALandPoint
{
	GENERATED_BODY()
	
public:
	virtual void DebugDraw() override;

	float GetXLength() { return XLength; }
	float GetYLength() { return YLength; }

private:
	UPROPERTY(EditAnywhere)
		float XLength = 100.0f;
	UPROPERTY(EditAnywhere)
		float YLength = 100.0f;
};
