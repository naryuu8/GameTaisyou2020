// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseUI.generated.h"
UENUM(BlueprintType)
enum class PauseState : uint8
{
	GAMEBACK,
	RESTART,
	SCORE,
	STAGESELECT,
};

UCLASS()
class WAVE_API UPauseUI : public UUserWidget
{
	GENERATED_BODY()
private:
	virtual void NativeConstruct() override;
	int SelectNumber;
	bool IsNoInput;//true時インプットを受け付けない
protected:
	//引数で一致する番号と同じセレクト番号になったら色を変える
	UFUNCTION(BlueprintCallable, Category = "C++Library")
		FSlateColor SelectTextColor(const PauseState state);
	//アニメーション再生中はポーズテキストを表示しない
	UFUNCTION(BlueprintCallable, Category = "C++Library")
		ESlateVisibility GetPauseTextVisibility();
public:
	void NextSelectState();
	void BackSelectState();
	//対応する選択ステートのアクションを実行する
	void SelectStateAction();
	UFUNCTION(BlueprintCallable, Category = "C++Library")
		FORCEINLINE	int GetSelectNumber() const{ return SelectNumber; };
	UPROPERTY(BlueprintReadWrite, Category = "C++Class")
		bool IsPlayAnimation;//アニメーション再生中ならtrue
	//呼び出し時にBPでアニメーション再生
	UFUNCTION(Category = "C++Event", BlueprintImplementableEvent, BlueprintCallable)
		void InitPlayAnimation();
	UFUNCTION(Category = "C++Event", BlueprintImplementableEvent, BlueprintCallable)
		void EndPlayAnimation();
	//スタンプテスト用イベント
	UFUNCTION(Category = "C++Event", BlueprintImplementableEvent, BlueprintCallable)
		void TestStampPlayAnimation();
	//ポーズ画面のバーため現在のプレイヤーの最大HPを受け取る
	UFUNCTION(Category = "C++Event", BlueprintImplementableEvent, BlueprintCallable)
		void SetMaxHP(const float hp);
	//ポーズ画面のバー更新のため現在のプレイヤーHPを受け取る
	UFUNCTION(Category = "C++Event", BlueprintImplementableEvent, BlueprintCallable)
		void SetHP(const float hp);
	//フェードインを行いリトライする
	UFUNCTION(Category = "C++Event", BlueprintImplementableEvent, BlueprintCallable)
		void Retry();
	//フェードインを行いステージ選択画面に戻る
	UFUNCTION(Category = "C++Event", BlueprintImplementableEvent, BlueprintCallable)
		void StageSelectChenge();
	UFUNCTION(BlueprintCallable, Category = "C++Library")
		FORCEINLINE	bool GetIsPlayAnimation() const{ return IsPlayAnimation; };
};
