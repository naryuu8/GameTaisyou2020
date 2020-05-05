// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WaveSpawner.generated.h"

UCLASS()
class WAVE_API AWaveSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWaveSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, Category = "Instanced")
		float WaveTime;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
private:
	int FrameCounter;
	int WaveCountTime;//”g‚ð‹N‚±‚·ŽžŠÔ
};
