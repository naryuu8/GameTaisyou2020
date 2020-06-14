// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseUI.generated.h"
UENUM(BlueprintType)
enum class PauseSelectState : uint8
{
	GAMEBACK,
	RESTART,
	STAGESELECT
};

UCLASS()
class WAVE_API UPauseUI : public UUserWidget
{
	GENERATED_BODY()
private:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	int SelectNumber = 0;
	bool IsNoInput;//true時インプットを受け付けない
protected:
	//アニメーション再生中はポーズテキストを表示しない
	UFUNCTION(BlueprintCallable, Category = "C++Library")
		ESlateVisibility GetPauseTextVisibility();
public:
	void NextSelectState();
	void BackSelectState();
	//対応する選択ステートのアクションを実行する
	void SelectStateAction();
	void EndAnimation();
	UFUNCTION(BlueprintCallable, Category = "C++Library")
		FORCEINLINE	int GetSelectNumber() const{ return SelectNumber; };
	UPROPERTY(BlueprintReadWrite, Category = "C++Class")
		bool IsPlayAnimation;//アニメーション再生中ならtrue
	//呼び出し時にBPでアニメーション再生
	UFUNCTION(Category = "C++Event", BlueprintImplementableEvent, BlueprintCallable)
		void InitPlayAnimation();
	UFUNCTION(Category = "C++Event", BlueprintImplementableEvent, BlueprintCallable)
		void EndPlayAnimation();
	//ポーズ画面の時計のため制限時間を受け取る
	UFUNCTION(Category = "C++Event", BlueprintImplementableEvent, BlueprintCallable)
		void SetTimeLimit(const int time);
	UFUNCTION(Category = "C++Event", BlueprintImplementableEvent, BlueprintCallable)
		void SetNormaTime(const int time);
	//ポーズ画面の時計のためノルマの針の位置を受け取る
	UFUNCTION(Category = "C++Event", BlueprintImplementableEvent, BlueprintCallable)
		void SetNormaAngle(const float time);
	//ポーズ画面の時計のため現在の針の位置を受け取る
	UFUNCTION(Category = "C++Event", BlueprintImplementableEvent, BlueprintCallable)
		void SetNeedleAndBG_Material(const float angle);
	//フェードインを行いリトライする
	UFUNCTION(Category = "C++Event", BlueprintImplementableEvent, BlueprintCallable)
		void Retry();
	//フェードインを行いステージ選択画面に戻る
	UFUNCTION(Category = "C++Event", BlueprintImplementableEvent, BlueprintCallable)
		void StageSelectChenge();
	//キーを押したとき項目画像サイズを変更
	UFUNCTION(Category = "C++Event", BlueprintImplementableEvent, BlueprintCallable)
		void ImageSizeChenge();
	UFUNCTION(BlueprintCallable, Category = "C++Library")
		FORCEINLINE	bool GetIsPlayAnimation() const{ return IsPlayAnimation; };
};
