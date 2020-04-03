// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CircleLandPoint.generated.h"

UCLASS()
class WAVE_API ACircleLandPoint : public AActor
{
	GENERATED_BODY()
	
public:
	float GetRadius() { return Radius; }
	// Sets default values for this actor's properties
	ACircleLandPoint();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere)
		float Radius = 30;
};
