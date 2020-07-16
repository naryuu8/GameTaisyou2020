// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Wave/WaterSurface/SquareLand.h"
#include "GameFramework/Actor.h"
#include "Scaffold.generated.h"

UCLASS()
class WAVE_API AScaffold : public ASquareLand
{
	GENERATED_BODY()

public:

protected:
	const float RiseTime = 1;
	const float	SinkZ = -300;
	float AppearZ;
	bool IsClear;
	bool IsFinish;
	
public:	
	// Sets default values for this actor's properties
	AScaffold();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void Rise(float DeltaTime);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Clear();
};
