// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnimInstance.generated.h"

DECLARE_DELEGATE(AttackEndDelegate);

UCLASS()
class WAVE_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
protected:
	UPROPERTY(BlueprintReadWrite, Category = "C++Class")
		float Speed;

public:
	UPROPERTY(BlueprintReadWrite, Category = "C++Class")
		bool IsCharge;
	UPROPERTY(BlueprintReadWrite, Category = "C++Class")
		bool IsAttack;
	UPROPERTY(BlueprintReadWrite, Category = "C++Class")
		bool IsClear;
	UPROPERTY(BlueprintReadWrite, Category = "C++Class")
		bool IsDeth;
	UPROPERTY(BlueprintReadWrite, Category = "C++Class")
		bool IsGameOver;
private:
	//UAnimMontage* AnimMontage;
	FAnimNode_StateMachine * AnimState;
public:
	// コンストラクタ
	UPlayerAnimInstance(const FObjectInitializer& ObjectInitializer);

	// アタック終了時にハンマーの先端部分を引数に取得
	AttackEndDelegate AttackEndCallBack;

	// アニメーションの初期化時
	virtual void NativeInitializeAnimation() override;
	// アニメーションの更新時
	virtual void NativeUpdateAnimation(float DeltaTime) override;
	// アニメーション評価後
	virtual void NativePostEvaluateAnimation() override;
	// アニメーションの終了化時
	virtual void NativeUninitializeAnimation() override;

	// イベント開始時
	virtual void NativeBeginPlay() override;

	bool IsCurrentStateName(FName StateName) const;
	bool IsChargeAnim() { return IsCurrentStateName("Charge State"); };


	// ブループリント側でアタック終了時にハンマーの先端部分を引数に入れる
	UFUNCTION(BlueprintCallable, Category = "C++Library")
		void AttackAnimEnd();
};
