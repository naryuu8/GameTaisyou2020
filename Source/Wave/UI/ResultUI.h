// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ResultUI.generated.h"

UENUM(BlueprintType)
enum class ResultState : uint8
{
	RESULT_ANIME,
	CLEAR_INANIME,
	NIMOTU_ANIME,
	GAUGE_ANIME,
	STAMP_ANIME,
	ENTER_WAIT,
	INPUT,
	END,
};
UENUM(BlueprintType)
enum class ResultSelectState : uint8
{
	NEXTSTAGE,
	STAGESELECT,
};
UCLASS()
class WAVE_API UResultUI : public UUserWidget
{
	GENERATED_BODY()
private:
	virtual void NativeConstruct() override;
	int SelectNumber = 0;
protected:
	//引数で一致する番号と同じセレクト番号になったら色を変える
	UFUNCTION(BlueprintCallable, Category = "C++Library")
		FSlateColor SelectTextColor(const ResultSelectState state);
public:
	void NextSelectState();
	void BackSelectState();
	//対応する選択ステートのアクションを実行する
	void SelectStateAction();
	UPROPERTY(BlueprintReadWrite, Category = "C++Class")
		FString NextStagePath;
	UPROPERTY(BlueprintReadWrite, Category = "C++Class")
		bool IsPlayAnimation = false;//アニメーション再生中ならtrue
	UPROPERTY(BlueprintReadWrite, Category = "C++Class")
		bool IsDataTable = false;//データテーブルを読み込めたらtrue
	UPROPERTY(BlueprintReadWrite, Category = "C++Class")
		ResultState State;//リザルトのステート状態保持
	UPROPERTY(BlueprintReadWrite, Category = "C++Class")
		ResultSelectState SelectState;
	UFUNCTION(Category = "C++Event", BlueprintImplementableEvent, BlueprintCallable)
		void ClearPlayAnimation();
	//このステージをクリアした時のMAXHP、HP、ノルマパーセントをセット
	UFUNCTION(Category = "C++Event", BlueprintImplementableEvent, BlueprintCallable)
		void SetResultGaugeAnimeCheckEvent(const float MaxHP,const float HP,const float NormaPercent);
	//このステージで取得した荷物をセット
	UFUNCTION(Category = "C++Event", BlueprintImplementableEvent, BlueprintCallable)
		void SetResultNowNimotuCheckEvent(const int NowNimotu);
	//このステージのスタンプアニメーションに必要な情報をセット
	UFUNCTION(Category = "C++Event", BlueprintImplementableEvent, BlueprintCallable)
		void SetStampAnimeCheckEvent(const int NormaNimotu,const int MaxNimotu);
	//フェードインを行い次のステージに行く
	UFUNCTION(Category = "C++Event", BlueprintImplementableEvent, BlueprintCallable)
		void NextStageChenge();
	//フェードインを行いステージ選択画面に戻る
	UFUNCTION(Category = "C++Event", BlueprintImplementableEvent, BlueprintCallable)
		void StageSelectChenge();
	//データテーブルが存在するか確認
	UFUNCTION(Category = "C++Event", BlueprintImplementableEvent, BlueprintCallable)
		void CheckDataTable();
	UFUNCTION(BlueprintCallable, Category = "C++Library")
		void SetStageSelectState();
	UFUNCTION(BlueprintCallable, Category = "C++Library")
		FORCEINLINE	ResultState GetResultState() const { return State; };
};
