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
	//他の計算式でここで求めた値を使うので必ず最初に呼ぶこと
	UFUNCTION(BlueprintCallable, Category = "C++Library")
		void SetOneFrameAngle(const int time_limit);
	//カウントダウンUIを表示する時間をセット
	UFUNCTION(BlueprintCallable, Category = "C++Library")
		void SetCountDownAngle(const int countdown);
	//ノルマ針の回転値をセット
	//戻り値は回転値
	UFUNCTION(BlueprintCallable, Category = "C++Library")
		float SetNormaAngle(const int norma_time);
	UFUNCTION(Category = "C++Event", BlueprintImplementableEvent, BlueprintCallable)
		void UpDateTime();
	UFUNCTION(Category = "C++Event", BlueprintImplementableEvent, BlueprintCallable)
		void SetTimeLimit(const int limit);
	UFUNCTION(Category = "C++Event", BlueprintImplementableEvent, BlueprintCallable)
		void SetCountDownTime(const int time);
	UFUNCTION(Category = "C++Event", BlueprintImplementableEvent, BlueprintCallable)
		void SetNormaTime(const int time);
};
