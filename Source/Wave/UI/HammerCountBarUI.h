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
		float DefaultHpSizeX;
	UPROPERTY(BlueprintReadWrite, Category = "C++Class")
		float DefaultHpSizeY;
	UPROPERTY(BlueprintReadWrite, Category = "C++Class")
		bool IsDamage;
	UPROPERTY(BlueprintReadWrite, Category = "C++Class")
		bool IsDamageDown;//ダメージゲージが減っている時true
public:
	//ハンマーを構えた開始を通知イベント
	UFUNCTION(Category = "C++Event", BlueprintImplementableEvent, BlueprintCallable)
		void FirstEvent();
	//現在のHPを受け取り減ってるゲージを表示する
	UFUNCTION(Category = "C++Event", BlueprintImplementableEvent, BlueprintCallable)
		void UpdateGauge(const float HammerHP);
	//ダメージゲージを減らすのを開始するイベント
	UFUNCTION(Category = "C++Event", BlueprintImplementableEvent, BlueprintCallable)
		void ReflectionGauge();
	//毎フレーム減らすゲージ量を計算し返す
	UFUNCTION(BlueprintCallable, Category = "C++Library")
		float DownGauge(const float DamageX,const float HpX);
	void SetMaxHammerHP(const float hp) { MaxHammerHP = hp; }
};
