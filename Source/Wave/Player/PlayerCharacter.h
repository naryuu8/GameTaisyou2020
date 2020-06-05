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
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool IsDeth;	// 場外に出たまたは水に落ちた時

private:
	// ポーズUI　エディタで指定する
	UPROPERTY(EditAnywhere)
		TSubclassOf<UPauseUI> PauseUIClass;
	UPROPERTY(EditAnywhere)
		TSubclassOf<UHammerCountBarUI> HammerCountBarUIClass;
	UPauseUI* PauseUI = nullptr;
	UHammerCountBarUI* HammerCountBarUI = nullptr;
	//ゲージで表示する用のプレイヤーのハンマーMAXHP
	UPROPERTY(EditAnywhere, Category = "Parameter")
		float MaxHammerHP = 100.0f;
	//1フレーム毎に溜めるハンマーパワー
	UPROPERTY(EditAnywhere, Category = "Parameter")
		float ChargeSpeed = 0.7f;
	// 一回で溜められる最大パワー
	UPROPERTY(EditAnywhere, Category = "Parameter")
		float ChargePowerMax = 100.0f;
	// CoolTime回復時間倍率（高いほど早い）
	UPROPERTY(EditAnywhere, Category = "Parameter")
		float CoolTimeHealSpped = 0.8f;
	// HP回復時間（高いほど早い）
	UPROPERTY(EditAnywhere, Category = "Parameter")
		float HpHealSpped = 0.9f;
	// 溜めている力をカウント
	float ChargeCount= 0.0f;
	//叩けない時間
	float CoolTime = 0.0f;
	// 波を起こす力
	UPROPERTY(EditAnywhere, Category = "Parameter")
		float HammerPowerValue = 1.0f;
	// プレイヤーの当たり判定の半径
	UPROPERTY(EditAnywhere, Category = "Parameter")
		float Radius = 100.0f;

	//UPROPERTY(EditAnywhere, Category = "Effect")
	//	bool  Now = 100.0f;

	float HammerHP;
	float MoveAmount;	// 移動量の割合

	void PauseInput();
	void CreateHammerCountBarUI();
	void UpdateGaugeHP();
	AWaterSurface* Water;

	ARaft* CurrentRaft = nullptr;	// 乗っていない時は常にnullptr
	bool IsInRaft = false;
	void ResetRaftParam();

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
		void OutLineDraw();
	UFUNCTION(BlueprintCallable, Category = "C++Library", BlueprintImplementableEvent)
		void OutLineNotDraw();
	UFUNCTION(BlueprintCallable, Category = "C++Library", BlueprintImplementableEvent)
		void PlayerDeth();

	UFUNCTION(BlueprintCallable, Category = "C++Library", BlueprintImplementableEvent)
		void ChageCreateEmmiter();
	UFUNCTION(BlueprintCallable, Category = "C++Library", BlueprintImplementableEvent)
		void ChageDestroyEmmiter();
	UFUNCTION(BlueprintCallable, Category = "C++Library", BlueprintImplementableEvent)
		void ChageUpDateEmmiter(FVector pos);


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

	void Move(const FVector & Direction, float Value);

	bool CheckTraceGround(FHitResult & Result, const FVector & Start, float SphereRadius, AActor* Ignore = nullptr);

	//キーを押したときのハンマー攻撃
	void TriggerHammerAttack(void);
	//キーを離したときのハンマー攻撃
	void ReleaseHammerAttack(void);

	void HummerAttackEnd();
protected:

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
	//ハンマー消費ゲージをマイナス
	void MinusHammerGauge(const float Power);
	// プレイヤーから下にレイを飛ばして陸の上かどうか調べる
	bool CheckGround();
	// カメラの向きから移動方向を算出
	FVector GetInputDirection(float VerticalValue, float HolizontalValue);
	// 移動方向にプレイヤーを向かせる関数
	void SetLookAt(FVector Direction, float Speed);
public:
	//HPバーのノルマ位置をセット
	void SetNormaPercent(const float percent);
	float GetMoveAmount() { return MoveAmount; };
	//ゲージUIを非表示にする
	void HammerCountBarParent();
	//プレイヤーを非表示にする
	UFUNCTION(BlueprintCallable, Category = "C++Library")
	void SetPlayerHiddenInGame();
	UFUNCTION(BlueprintCallable, Category = "C++Library")
		FORCEINLINE	float GetMaxHammerHP() const { return MaxHammerHP; };
	UFUNCTION(BlueprintCallable, Category = "C++Library")
		FORCEINLINE	float GetHammerHP() const { return HammerHP; };
	UFUNCTION(BlueprintCallable, Category = "C++Library")
		FORCEINLINE	float GetHammerPower() const { return HammerPower; };
	UFUNCTION(BlueprintCallable, Category = "C++Library")
		FORCEINLINE	float GetChargeCount() const { return ChargeCount; };
	UFUNCTION(BlueprintCallable, Category = "C++Library")
		FORCEINLINE	bool GetIsAttackHold() const { return IsAttackHold; };
};