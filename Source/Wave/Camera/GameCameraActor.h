// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameCameraActor.generated.h"

UCLASS()
class WAVE_API AGameCameraActor : public AActor
{
	GENERATED_BODY()
	
public:	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UGameCameraComponent * GameCameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent * Camera;

	// Sets default values for this actor's properties
	AGameCameraActor();

	// イベント：カメラのバイブレーション
	UFUNCTION(BlueprintCallable, Category = "C++Event", BlueprintImplementableEvent)
		void EventCameraShake(float Scale);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
