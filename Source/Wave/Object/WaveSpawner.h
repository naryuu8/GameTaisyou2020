// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
// 一定時間ごとに波を起こすクラス
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
	UPROPERTY(EditAnywhere, Category = "Instanced")
		float WavePower = 0.1f;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
private:
	int FrameCounter;
	int WaveCountTime;//波を起こす時間
};
