// Fill out your copyright notice in the Description page of Project Settings.
// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
//generated.hは一番最後にかかないといけない
#include "PlayerCharacter.generated.h"
class UPauseUI;
class UPlayerAnimInstance;
UCLASS(config = Game)
class APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class AGameCameraActor* FollowCamera;
private:
	// ポーズUI　エディタで指定する
	UPROPERTY(EditAnywhere)
		TSubclassOf<UPauseUI> UIClass;
	UPauseUI* PauseUI;
	void PauseInput();
public:
	APlayerCharacter();

	//virtual void BeginPlay() override;
	UFUNCTION(BlueprintCallable, Category = "C++Library")
		void BeginPlay_C();

	UFUNCTION(BlueprintCallable, Category = "C++Library")
		FORCEINLINE	int GetAttackCount() { return AttackCount; };

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++Class")
		float BaseTurnRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++Class")
		float BaseLookUpRate;

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
	int AttackCount = 0;

	//水面に波をたてる
	void WaterAttack();
	//ハンマー残り回数をマイナス
	void MinusHammerCount();
public:
	/** Returns CameraBoom subobject **/
//	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
//	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};