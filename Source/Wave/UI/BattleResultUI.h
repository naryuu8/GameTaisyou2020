// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BattleResultUI.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class BattleResultState : uint8
{
	RESTART,
	STAGESELECT,
};
UCLASS()
class WAVE_API UBattleResultUI : public UUserWidget
{
	GENERATED_BODY()
private:
	int SelectNumber = 0;
	bool IsNoInput = false;//true時インプットを受け付けない
public:
	//1Pが勝ったらtrue
	UFUNCTION(Category = "C++Event", BlueprintImplementableEvent, BlueprintCallable)
		void SetWinPlayer(const bool player_one_win);
	void NextSelectState();
	void BackSelectState();
	//対応する選択ステートのアクションを実行する
	void SelectStateAction();
	UFUNCTION(BlueprintCallable, Category = "C++Library")
		FORCEINLINE	int GetSelectNumber() const { return SelectNumber; };
	//キーを押したとき項目画像サイズを変更
	UFUNCTION(Category = "C++Event", BlueprintImplementableEvent, BlueprintCallable)
		void ImageSizeChenge();
	//フェードインを行いリトライする
	UFUNCTION(Category = "C++Event", BlueprintImplementableEvent, BlueprintCallable)
		void RetryScene();
	//フェードインを行いステージ選択画面に戻る
	UFUNCTION(Category = "C++Event", BlueprintImplementableEvent, BlueprintCallable)
		void StageSelectChenge();
};
