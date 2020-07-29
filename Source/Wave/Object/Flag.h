// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Flag.generated.h"

UCLASS()
class WAVE_API AFlag : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFlag();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BluePrintReadWrite)
		int BattleNumber;

	UFUNCTION(Category = "C++Event", BlueprintCallable)
		void OnPlayerCheck(class AActor* OtherActor);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	int GetNumber() { return BattleNumber; }

	bool IsGoal;
};
