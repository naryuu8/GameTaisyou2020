// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HammerCountBarUI.generated.h"

/**
 * 
 */
UCLASS()
class WAVE_API UHammerCountBarUI : public UUserWidget
{
	GENERATED_BODY()
private:
	float NowHp;//徐々にゲージを減らすために今のHPゲージと減る前のHPの差分を格納する
protected:
	UPROPERTY(BlueprintReadWrite, Category = "C++Class")
		float MaxHammerHP;
	UPROPERTY(BlueprintReadWrite, Category = "C++Class")
		float GaugeHP;
	UPROPERTY(BlueprintReadWrite, Category = "C++Class")
		float MaxChargePowerMax;//プレイヤーがチャージできる最大量
	//プレイヤークリア時のHP
	UPROPERTY(BlueprintReadWrite, Category = "C++Class")
		float ClearHammerHP;
	//ゲージ上昇アニメが終わったらTrue（リザルトで使用）
	UPROPERTY(BlueprintReadWrite, Category = "C++Class")
		bool IsGaugeAnimeEnd = false;
	UPROPERTY(BlueprintReadWrite, Category = "C++Class")
		float DefaultHpSizeX;
	UPROPERTY(BlueprintReadWrite, Category = "C++Class")
		float DefaultHpSizeY;
	UPROPERTY(BlueprintReadWrite, Category = "C++Class")
		bool IsDamage = false;
	UPROPERTY(BlueprintReadWrite, Category = "C++Class")
		bool IsDamageDown = false;//ダメージゲージが減っている時true
public:
	//ハンマーを構えた開始を通知イベント
	UFUNCTION(Category = "C++Event", BlueprintImplementableEvent, BlueprintCallable)
		void FirstEvent();
	//現在のHPを受け取り減ってるゲージを表示する
	UFUNCTION(Category = "C++Event", BlueprintImplementableEvent, BlueprintCallable)
		void UpdateGauge(const float HammerHP);
	UFUNCTION(Category = "C++Event", BlueprintImplementableEvent, BlueprintCallable)
		void UpdateDamageGauge(const float HammerHP);
	//現在のHPを受け取り減ってるゲージを表示する(ポーズの時は赤ゲージを表示しないようにする
	UFUNCTION(Category = "C++Event", BlueprintImplementableEvent, BlueprintCallable)
		void UpdatePauseGauge(const float HammerHP);
	//クールタイム時間を受け取りアイコンに反映させる
	UFUNCTION(Category = "C++Event", BlueprintImplementableEvent, BlueprintCallable)
		void UpdateCoolTime(const float time);
	//ダメージゲージを減らすのを開始するイベント
	UFUNCTION(Category = "C++Event", BlueprintImplementableEvent, BlueprintCallable)
		void ReflectionGauge();
	//毎フレーム減らすゲージ量を計算し返す
	UFUNCTION(BlueprintCallable, Category = "C++Library")
		float DownGauge(const float DamageX,const float HpX);
	//ノルマバーの位置をセット
	UFUNCTION(Category = "C++Event", BlueprintImplementableEvent, BlueprintCallable)
		void SetNormaPercent(const float Percent);
	UFUNCTION(BlueprintCallable, Category = "C++Library")
		void SetMaxHammerHP(const float hp) { MaxHammerHP = hp; }
	UFUNCTION(BlueprintCallable, Category = "C++Library")
		void SetMaxChargePowerMax(const float charge) { MaxChargePowerMax = charge; }
	UFUNCTION(BlueprintCallable, Category = "C++Library")
		FORCEINLINE bool GetIsGaugeAnimeEnd() const { return IsGaugeAnimeEnd; }
	UFUNCTION(BlueprintCallable, Category = "C++Library")
		FORCEINLINE	float GetGaugeHP() const { return GaugeHP; };
};
