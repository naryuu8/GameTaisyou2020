// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../SaveData.h"
#include "../SoundManager.h"
#include "BackGroundUI.generated.h"

/**
 * 
 */
UCLASS()
class WAVE_API UBackGroundUI : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, Category = "C++Class")
		UAudioComponent* AudioComponent;
	UPROPERTY(BlueprintReadWrite, Category = "C++Class")
		FText MaxNimotu;
	UPROPERTY(BlueprintReadWrite, Category = "C++Class")
		FText InNimotu;
	UPROPERTY(BlueprintReadWrite, Category = "C++Class")
		FText LimitTime;
	//スタンプ用テキスト
	UPROPERTY(BlueprintReadWrite, Category = "C++Class")
		FText StampNimotu;
	UPROPERTY(BlueprintReadWrite, Category = "C++Class")
		FText StampPercent;
	UPROPERTY(BlueprintReadWrite, Category = "C++Class")
		FSaveDataStruct ResultSaveData;
	//リザルト時入った荷物を取得
	UPROPERTY(BlueprintReadWrite, Category = "C++Class")
		int CountMaxInNimotu = 0;
	//リザルト時入った荷物を0からカウント
	UPROPERTY(BlueprintReadWrite, Category = "C++Class")
		int CountInNimotu = 0;
	//リザルト時のノルマの荷物
	UPROPERTY(BlueprintReadWrite, Category = "C++Class")
		int CountNormaNimotu = 0;
	//リザルト時の荷物の最大数
	UPROPERTY(BlueprintReadWrite, Category = "C++Class")
		int CountMaxNumNimotu = 0;
	//カウントアニメが終わったらtrue
	UPROPERTY(BlueprintReadWrite, Category = "C++Class")
		bool IsCountAnimeEnd = false;
	//スタンプアニメが終わったらtrue
	UPROPERTY(BlueprintReadWrite, Category = "C++Class")
		bool IsStampAnimeEnd = false;
	UPROPERTY(BlueprintReadWrite, Category = "C++Class")
		float ResultClearAngle;
	UPROPERTY(BlueprintReadWrite, Category = "C++Class")
		float ResultNormaAngle;
	UPROPERTY(BlueprintReadWrite, Category = "C++Class")
		int ResultNormaTime;
	//全てのスタンプが押されていたらtrueを返す
	UFUNCTION(BlueprintCallable, Category = "C++Library")
		bool GetIsAllStamp() const;
	UFUNCTION(BlueprintCallable, Category = "C++Library")
		FORCEINLINE	bool GetIsCountAnimeEnd() const { return IsCountAnimeEnd; }
	UFUNCTION(BlueprintCallable, Category = "C++Library")
		FORCEINLINE	bool GetIsStampAnimeEnd() const { return IsStampAnimeEnd; }
	UFUNCTION(BlueprintCallable, Category = "C++Library")
		bool GetIsStamp(const int index) const;
	UFUNCTION(Category = "C++Event", BlueprintImplementableEvent, BlueprintCallable)
		void SetTimeLimit(const int time);
	UFUNCTION(Category = "C++Event", BlueprintImplementableEvent, BlueprintCallable)
		void SetNormaAngle(const float time);
	UFUNCTION(Category = "C++Event", BlueprintImplementableEvent, BlueprintCallable)
		void SetNeedleAndBG_Material(const float angle);
	UFUNCTION(Category = "C++Event", BlueprintImplementableEvent, BlueprintCallable)
		void NimotuCountPlayAnimation();
	UFUNCTION(Category = "C++Event", BlueprintImplementableEvent, BlueprintCallable)
		void SetNormaTime(const int time);
	//セーブデータの指定したスタンプフラグをtrueにする
	UFUNCTION(BlueprintCallable, Category = "C++Library")
		void SetSaveDataIsStamp(const int index) { ResultSaveData.IsStamp[index] = true;}
};
