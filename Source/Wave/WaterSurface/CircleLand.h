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

private:
	UPROPERTY(EditAnywhere)
		float Radius = 100.0f;
};
