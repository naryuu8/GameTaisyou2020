// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameCamera.generated.h"

UCLASS(hideCategories = (Mobility))
class WAVE_API AGameCamera : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGameCamera();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USceneComponent * CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent * FollowCamera;

protected:
	// Called when the game starts or when spawned
	void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
		float FollowDistance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
		float RotationSpeed = 1.0f;
private:

	UPROPERTY(EditAnywhere, Category = Camera, meta = (AllowPrivateAccess = "true"))
		float VerticalMax;
	UPROPERTY(EditAnywhere, Category = Camera, meta = (AllowPrivateAccess = "true"))
		float VerticalMin;
	// 極位方向の最小値・最大値角度

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void UpdateRotation();
};
