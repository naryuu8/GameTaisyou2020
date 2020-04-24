// Fill out your copyright notice in the Description page of Project Settings.
// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnimInstance.h"
//generated.hは一番最後にかかないといけない
#include "PlayerCharacter.generated.h"

UCLASS(config = Game)
class APlayerCharacter : public ACharacter
{
	GENERATED_BODY()
		/** Camera boom positioning the camera behind the character */
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;


public:
	APlayerCharacter();

	//virtual void BeginPlay() override;
	UFUNCTION(BlueprintCallable, Category = "C++Library")
		void BeginPlay_C();
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
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

	UFUNCTION(BlueprintCallable, Category = "C++Library")
	void TriggerHammerAttack();
	//キーを離したときのハンマー攻撃
	UFUNCTION(BlueprintCallable, Category = "C++Library")
	void ReleaseHammerAttack();
	UFUNCTION(BlueprintCallable, Category = "C++Library")
	void WaterAttack2(FVector position);
private:
	UPlayerAnimInstance* AnimInst;
	//水面に波をたてる
	void WaterAttack();
public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};