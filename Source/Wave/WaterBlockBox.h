// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "WaterBlockBox.generated.h"
class APlayerCharacter;
/**
 * プレイヤーが穴に落下した時穴の外に落ちないようにするBox
 */
UCLASS()
class WAVE_API AWaterBlockBox : public ATriggerBox
{
	GENERATED_BODY()
private:
	APlayerCharacter* GetPlayer;//プレイヤー情報
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
public:
	UFUNCTION(Category = "C++Event", BlueprintImplementableEvent, BlueprintCallable)
		void SetNoCollision();
	UFUNCTION(Category = "C++Event", BlueprintImplementableEvent, BlueprintCallable)
		void SetONCollision();
};
