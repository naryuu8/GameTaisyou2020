// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WaterWheel.generated.h"
//波を受けたら回る水車クラス
class AWaterSurface;
UCLASS()
class WAVE_API AWaterWheel : public AActor
{
	GENERATED_BODY()

private:
	bool IsRotation;//回転中true
	float WheelPower;
	float RotationTime;
	float Rot;
	float firstrot;
	bool IsUpRot;//回転値が正の値だったらtrue
	AWaterSurface* WaterSurface;
	float CheckFirstWheelRotation(FVector pos);
	void CheckReverseWheelRotation(FVector pos);//回っている時反対方向へ強い力が加えられたら反対に回す
public:	
	// Sets default values for this actor's properties
	AWaterWheel();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(BlueprintCallable, Category = "C++Library")
		float RotationTimeCheck(FVector pos);
	UFUNCTION(BlueprintCallable, Category = "C++Library")
		FORCEINLINE bool GetIsRotation() const { return IsRotation; }
	UFUNCTION(BlueprintCallable, Category = "C++Library")
		FORCEINLINE float GetWheelPower() const { return WheelPower; }
};
