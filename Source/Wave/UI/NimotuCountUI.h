// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NimotuCountUI.generated.h"

/**
 * 
 */
UCLASS()
class WAVE_API UNimotuCountUI : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, Category = "C++Class")
		FText StageNimotu;
	UPROPERTY(BlueprintReadWrite, Category = "C++Class")
		FText InNimotu;
	UPROPERTY(BlueprintReadWrite, Category = "C++Class")
		FText NormaNimotu;
	UPROPERTY(BlueprintReadWrite, Category = "C++Class")
		FText MaxNimotu;
	void SetStageNimotu(const int count);
	void SetStageInNimotu(const int count);
	void SetNormaNimotu(const int count);
	void SetMaxNimotu(const int count);
	UFUNCTION(Category = "C++Event", BlueprintImplementableEvent, BlueprintCallable)
		void GameNimotuAnimation();
	UFUNCTION(Category = "C++Event", BlueprintImplementableEvent, BlueprintCallable)
		void NimotuInAnimation();
	//ノルマを達成したらアニメーション中に文字色を赤色にする
	UFUNCTION(Category = "C++Event", BlueprintImplementableEvent, BlueprintCallable)
		void NormaInAnimation();
	//ノルマを達成していてノルマ以下になったらアニメーション中に文字色を元に戻す
	UFUNCTION(Category = "C++Event", BlueprintImplementableEvent, BlueprintCallable)
		void NormaNoAnimation();
};
