// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"
#include "EnvSound.generated.h"

UCLASS()
class WAVE_API AEnvSound : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnvSound();
	UPROPERTY(EditAnywhere)
	USoundBase* soundBase;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void Play3DSound();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
