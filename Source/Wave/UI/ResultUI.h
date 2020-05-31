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
	BAR_ANIME,
	STAMP_ANIME,
	INPUT,
};
UCLASS()
class WAVE_API UResultUI : public UUserWidget
{
	GENERATED_BODY()
private:
	virtual void NativeConstruct() override;
public:
	UPROPERTY(BlueprintReadWrite, Category = "C++Class")
		bool IsPlayAnimation;//アニメーション再生中ならtrue
	UPROPERTY(BlueprintReadWrite, Category = "C++Class")
		ResultState State;//リザルトのステート状態保持
	UFUNCTION(Category = "C++Event", BlueprintImplementableEvent, BlueprintCallable)
		void ClearPlayAnimation();
};
