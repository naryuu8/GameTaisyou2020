// Fill out your copyright notice in the Description page of Project Settings.
// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Niagara/Classes/NiagaraSystem.h"	// エフェクト用
//generated.hは一番最後にかかないといけない
#include "PlayerCharacter.generated.h"
class UPauseUI;
class UPlayerAnimInstance;
class UHammerCountBarUI;
class AWaterSurface;
class ARaft;

UCLASS(config = Game)
class APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Camera)
		class AGameCameraActor* FollowCamera;
	
private:
	// ポーズUI　エディタで指定する
	UPROPERTY(EditAnywhere)
		TSubclassOf<UPauseUI> PauseUIClass;
	UPROPERTY(EditAnywhere)
		TSubclassOf<UHammerCountBarUI> HammerCountBarUIClass;
	UPauseUI* PauseUI = nullptr;
	UHammerCountBarUI* HammerCountBarUI = nullptr;
	//ゲージで表示する用のプレイヤーのハンマーMAXHP
	UPROPERTY(EditAnywhere, Category = "Instanced")
		float MaxHammerHP = 100.0f;
	//1フレーム毎に溜めるハンマーパワー
	UPROPERTY(EditAnywhere, Category = "Instanced")
		float ChargePower = 0.1f;
	float HammerHP;
	void PauseInput();

	FVector PrevPos;

	AWaterSurface* Water;

	ARaft* CurrentRaft;
	bool IsRaftRiding;
public:
	APlayerCharacter();

	//virtual void BeginPlay() override;
	UFUNCTION(BlueprintCallable, Category = "C++Library")
		void BeginPlay_C();

	virtual void Tick(float DeltaTime) override;

	// ハンマーの先端のトランスフォーム
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "C++Class")
		USceneComponent* HummerTipPoint = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++Class")
		float BaseTurnRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++Class")
		float BaseLookUpRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
		UNiagaraSystem* AttackEffect = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
		float AttackEffectScale = 1.0f;

	UFUNCTION(BlueprintCallable, Category = "C++Library", BlueprintImplementableEvent)
		void OutLineDrow();
	UFUNCTION(BlueprintCallable, Category = "C++Library", BlueprintImplementableEvent)
		void OutLineNotDrow();


	static FORCEINLINE bool Trace(
		UWorld* World,
		AActor* ActorToIgnore,
		const FVector& Start,
		const FVector& End,
		FHitResult& HitOut,
		ECollisionChannel CollisionChannel = ECC_Pawn,
		bool ReturnPhysMat = false)
	{
		if (!World)
		{
			return false;
		}

		FCollisionQueryParams TraceParms(FName(TEXT("VictoreCore Trace")), true, ActorToIgnore);
		TraceParms.bTraceComplex = true;
		TraceParms.bReturnPhysicalMaterial = ReturnPhysMat;
		TraceParms.AddIgnoredActor(ActorToIgnore);
		HitOut = FHitResult(ForceInit);
		World->LineTraceSingleByChannel(
			HitOut,
			Start,
			End,
			CollisionChannel,
			TraceParms
			);


		return (HitOut.GetActor() != NULL);
	}

protected:

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);
	void TurnAtRate(float Rate);

	void LookUpAtRate(float Rate);
	//キーを押したときのハンマー攻撃
	void TriggerHammerAttack(void);
	//キーを離したときのハンマー攻撃
	void ReleaseHammerAttack(void);

	void HummerAttackEnd();


protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

	//BlueprintReadWrite=GetとSet関数をブルーブリントで公開

	//ハンマー攻撃でホールド中ならtrue
	UPROPERTY(BlueprintReadWrite, Category = "C++Class")
		bool IsAttackHold;
	//攻撃アニメ再生中ならtrue
	UPROPERTY(BlueprintReadWrite, Category = "C++Class")
		bool IsPlayAttackAnime;
	//ハンマーパワー変数
	UPROPERTY(BlueprintReadWrite, Category = "C++Class")
		float HammerPower;
private:

	UPlayerAnimInstance* AnimInst;

	//水面に波をたてる
	void WaterAttack(FVector Point, float Power);
	//ハンマー残り回数をマイナス
	void MinusHammerCount();
	//ハンマー消費ゲージをマイナス
	void MinusHammerGauge(const float Power);

	bool IsRide;
public:
	/** Returns CameraBoom subobject **/
//	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
//	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};