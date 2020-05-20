// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GlobalGameInstance.generated.h"

// グローバル変数クラス
//----------------------------------------------------
// シーンを跨いで保持する変数や関数があったらご自由に追加どうぞ
//----------------------------------------------------
//呼び出し方
// UGlobalGameInstance* instance = UGlobalGameInstance::GetInstance();
UCLASS()
class WAVE_API UGlobalGameInstance : public UGameInstance
{
	GENERATED_BODY()
private:
		int StageNumber;
public:
	static UGlobalGameInstance* GetInstance();
	//アニメーション再生中はポーズテキストを表示しない
	UFUNCTION(BlueprintCallable, Category = "GlobalC++Class")
		void SetStageNumber(const int number) { StageNumber = number; };
	UFUNCTION(BlueprintCallable, Category = "GlobalC++Class")
		FORCEINLINE int GetStageNumber() const { return StageNumber; }
};
