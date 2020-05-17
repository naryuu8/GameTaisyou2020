// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Rock.generated.h"
class AWaterSurface;

UCLASS()
class WAVE_API ARock : public AActor
{
	GENERATED_BODY()
private:
	AWaterSurface* Water;
	
public:	
	// Sets default values for this actor's properties
	ARock(); 

	UPROPERTY(EditAnywhere)
		float Magnification = 1.0f;

	bool IsWave;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
