// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TitlePlayer.generated.h"

UCLASS()
class WAVE_API ATitlePlayer : public ACharacter
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere)
		AActor* TargetPoint = nullptr;
	UPROPERTY(EditAnywhere)
		AActor* BattleTargetPoint = nullptr;
	UPROPERTY(EditAnywhere, Category = "Instanced")
		float MoveSpeed = 0.02f;//掲示板に向かう時の移動速度
public:
	// Sets default values for this character's properties
	ATitlePlayer();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(BlueprintReadWrite, Category = "C++Class")
		bool IsMove = false;
	//タイトルからエンターが押されたらtrue
	UPROPERTY(BlueprintReadWrite, Category = "C++Class")
		bool IsSelect = false;
	//ターゲットアクターに近づいたらtrue
	UPROPERTY(BlueprintReadWrite, Category = "C++Class")
		bool IsTarget = false;
public:	
	UFUNCTION(BlueprintCallable, Category = "C++Library")
		void BeginPlay_C();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void TargetRotation(const bool type);//ターゲットの方に向く
	void TargetMove(const bool type);//ターゲットの方に移動する
	UFUNCTION(BlueprintCallable, Category = "C++Library")
		void SetIsMove(const bool type) { IsMove = type; };
	UFUNCTION(BlueprintCallable, Category = "C++Library")
		void SetIsSelect(const bool type) { IsSelect = type; };
};
