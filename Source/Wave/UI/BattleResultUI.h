// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BattleResultUI.generated.h"

/**
 * 
 */
UCLASS()
class WAVE_API UBattleResultUI : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(Category = "C++Event", BlueprintImplementableEvent, BlueprintCallable)
		void SetPlayerScore(const int index,const int score);
	//1PÇ™èüÇ¡ÇΩÇÁtrue
	UFUNCTION(Category = "C++Event", BlueprintImplementableEvent, BlueprintCallable)
		void SetWinPlayer(const bool player_one_win);
};
