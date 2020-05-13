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
public:
	static UGlobalGameInstance* GetInstance();
	UPROPERTY(BlueprintReadWrite, Category = "GlobalC++Class")
		int StageNumber;
};
