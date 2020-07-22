// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Stake.generated.h"

UCLASS()
class WAVE_API AStake : public AActor
{
	GENERATED_BODY()
private:
	bool IsOn;
	
public:	
	// Sets default values for this actor's properties
	AStake();

	bool GetIsOn() { return IsOn; }

	void Push(const FVector Point);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
