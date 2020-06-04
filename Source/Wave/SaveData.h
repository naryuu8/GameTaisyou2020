// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SaveData.generated.h"
#define STAMP_NUM (3)//配達紙のスタンプの数
// セーブデータクラス
//----------------------------------------------------
// セーブデータに必要な構造体、変数を定義
//----------------------------------------------------
USTRUCT(BlueprintType)
struct FSaveDataStruct
{
	GENERATED_USTRUCT_BODY();
	FSaveDataStruct(){
		IsStamp.Init(false, STAMP_NUM);
	}
	//ステージに配達した数
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Weapon)
		int InNimotu = 0;

	//ステージをクリアした時のゲージ位置
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Weapon)
		float HpBarPercent = 0.0f;

	//スタンプの状態、押されていたらtrue
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Weapon)
		TArray<bool> IsStamp;

};
UCLASS()
class WAVE_API USaveData : public USaveGame
{
	GENERATED_BODY()
private:
	
public:
	UPROPERTY(BlueprintReadWrite, Category = "C++Class")
		TArray<FSaveDataStruct> Save;
	UFUNCTION(BlueprintCallable, Category = "C++Library")
		FSaveDataStruct GetSaveData(const int number) const;
};