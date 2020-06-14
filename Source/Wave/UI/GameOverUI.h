// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameOverUI.generated.h"
UENUM(BlueprintType)
enum class GameOverState : uint8
{
	RESTART,
	STAGESELECT,
};
/**
 * 
 */
UCLASS()
class WAVE_API UGameOverUI : public UUserWidget
{
	GENERATED_BODY()
private:
	int SelectNumber = 0;
public:
	UFUNCTION(BlueprintCallable, Category = "C++Library")
		FORCEINLINE	int GetSelectNumber() const { return SelectNumber; };
	UPROPERTY(BlueprintReadWrite, Category = "C++Class")
		bool IsPlayAnimation;//アニメーション再生中ならtrue
	UFUNCTION(BlueprintCallable, Category = "C++Library")
		FORCEINLINE	bool GetIsPlayAnimation() const { return IsPlayAnimation; }
	UFUNCTION(BlueprintCallable, Category = "C++Library")
		ESlateVisibility GetGameOverTextVisibility();
	UFUNCTION(BlueprintCallable, Category = "C++Library")
		void SwitchGameOverStateAction();
	void NextSelectState();
	void BackSelectState();
	//対応する選択ステートのアクションを実行する
	void SelectStateAction();
	//呼び出し時にBPでアニメーション再生
	UFUNCTION(Category = "C++Event", BlueprintImplementableEvent, BlueprintCallable)
		void InitPlayAnimation();
	UFUNCTION(Category = "C++Event", BlueprintImplementableEvent, BlueprintCallable)
		void EndPlayAnimation();
	//ゲームオーバー用プレイヤーモデル生成
	UFUNCTION(Category = "C++Event", BlueprintImplementableEvent, BlueprintCallable)
		void SpawnGameOverPlayer();
	UFUNCTION(Category = "C++Event", BlueprintImplementableEvent, BlueprintCallable)
		void ImageSizeChenge();
};
