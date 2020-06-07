// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TimeUI.generated.h"

/**
 * 
 */
UCLASS()
class WAVE_API UTimeUI : public UUserWidget
{
	GENERATED_BODY()
private:

public:
	//制限時間
	UPROPERTY(BlueprintReadWrite, Category = "C++Class")
		int LimitTime;
	//1フレームで回転する量
	UPROPERTY(BlueprintReadWrite, Category = "C++Class")
		float OneFrameAngle;
	//カウントダウンUIを表示する回転値
	UPROPERTY(BlueprintReadWrite, Category = "C++Class")
		float CountDownAngle;
	//ノルマを表示する回転値
	UPROPERTY(BlueprintReadWrite, Category = "C++Class")
		float NormaAngle;
	//1フレーム毎に回転する値を求める
	UFUNCTION(BlueprintCallable, Category = "C++Library")
		void SetOneFrameAngle(const int time_limit);
	//カウントダウンUIを表示する時間をセット
	//戻り値は引数をそのまま返す
	UFUNCTION(BlueprintCallable, Category = "C++Library")
		int SetCountDownAngle(const int countdown);
	//ノルマ針の回転値をセット
	//戻り値は回転値
	UFUNCTION(BlueprintCallable, Category = "C++Library")
		float SetNormaAngle(const int norma_time);
};
