// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "GameCameraActor.generated.h"

UCLASS()
class WAVE_API AGameCameraActor : public AActor
{
	GENERATED_BODY()
	
	enum class FollowType : int
	{
		FieldCenter = 0,
		CharacterFollow_Far,
		CharacterFollow_Near,
		Num
	};

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

	void SetFollowTarget(ACharacter * Target) { FollowTarget = Target; }

protected:
	// フィールドの中心座標（ゲームスタート時のカメラの中心座標）
	FVector FieldCenterPos;
	// カメラが追従するターゲット
	ACharacter * FollowTarget;

	FollowType Type = FollowType::CharacterFollow_Far;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
		float FieldDistance = 800.0f; // ステージ見降ろし視点でのカメラの距離
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
		float CharacterDistance_Far = 500.0f; // プレイヤー見降ろし視点でのカメラの距離
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
		float CharacterDistance_Near = 200.0f; // プレイヤー追従視点でのカメラの距離

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void InputChangeType();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
