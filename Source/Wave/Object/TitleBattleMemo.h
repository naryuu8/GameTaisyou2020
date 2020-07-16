// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TitleBattleMemo.generated.h"
class UFadeUI;
UCLASS()
class WAVE_API ATitleBattleMemo : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATitleBattleMemo();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere)
		FString StageLevelPath;
	UPROPERTY(EditAnywhere)
		TSubclassOf<UFadeUI> FadeUIClass;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void FadeStart();
};
