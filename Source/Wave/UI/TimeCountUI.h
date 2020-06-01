// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TimeCountUI.generated.h"

/**
 * 
 */
UCLASS()
class WAVE_API UTimeCountUI : public UUserWidget
{
	GENERATED_BODY()
private:
	// 終了時に呼ばれる（RemoveFromParentでも呼ばれる）
	virtual void NativeDestruct() override;
protected:
	UPROPERTY(BlueprintReadWrite, Category = "C++Class")
		int TimeCount = 0;
	//カウントが0になったらtrue
	UPROPERTY(BlueprintReadWrite, Category = "C++Class")
		bool IsCountZero = false;
	UPROPERTY(BlueprintReadWrite, Category = "C++Class")
		FText CountText;
public:
	void SetTimeCount(const int time) { TimeCount = time; }
	UFUNCTION(BlueprintCallable, Category = "C++Library")
		FORCEINLINE	bool GetIsCountZero() const { return IsCountZero; }
};
