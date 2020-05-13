// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class WAVE_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
protected:
	UPROPERTY(BlueprintReadWrite, Category = "C++Class")
		float Speed;
	UPROPERTY(BlueprintReadWrite, Category = "C++Class")
		bool IsAttackAnime;

private:
	UAnimMontage* AnimMontage;
public:
	// コンストラクタ
	UPlayerAnimInstance(const FObjectInitializer& ObjectInitializer);

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

	void HummerChergeEvent();
	void HummerAttackEvent();

	UFUNCTION(BlueprintCallable, Category = "C++Library")
	FORCEINLINE bool GetIsAttackAnime() const { return IsAttackAnime; }

};
