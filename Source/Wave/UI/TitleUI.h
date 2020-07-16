// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TitleUI.generated.h"
UENUM(BlueprintType)
enum class TitleSelectState : uint8
{
	GAME_START,
	BATTLE_MODE,
	DATA_DELETE,
	GAME_END,
};
/**
 * 
 */
UCLASS()
class WAVE_API UTitleUI : public UUserWidget
{
	GENERATED_BODY()
private:
	int SelectNumber = 0;
public:
	void NextSelectState();
	void BackSelectState();
	UFUNCTION(Category = "C++Event", BlueprintImplementableEvent, BlueprintCallable)
		void ImageSizeChenge();
	UFUNCTION(Category = "C++Event", BlueprintImplementableEvent, BlueprintCallable)
		void DeleteSaveData();
	UFUNCTION(Category = "C++Event", BlueprintImplementableEvent, BlueprintCallable)
		void EndGame();
	UFUNCTION(BlueprintCallable, Category = "C++Library")
		FORCEINLINE	int GetSelectNumber() const { return SelectNumber; };
};
