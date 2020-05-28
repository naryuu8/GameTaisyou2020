// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameOverUI.generated.h"
UENUM(BlueprintType)
enum class GameOverState : uint8
{
	RESTART,
	STAGESELECT,
};
/**
 * 
 */
UCLASS()
class WAVE_API UGameOverUI : public UUserWidget
{
	GENERATED_BODY()
private:
	int SelectNumber;
public:
	UFUNCTION(BlueprintCallable, Category = "C++Library")
		FORCEINLINE	int GetSelectNumber() const { return SelectNumber; };
	UPROPERTY(BlueprintReadWrite, Category = "C++Class")
		bool IsPlayAnimation;//�A�j���[�V�����Đ����Ȃ�true
	UFUNCTION(BlueprintCallable, Category = "C++Library")
		FORCEINLINE	bool GetIsPlayAnimation() const { return IsPlayAnimation; }
	//�����ň�v����ԍ��Ɠ����Z���N�g�ԍ��ɂȂ�����F��ς���
	UFUNCTION(BlueprintCallable, Category = "C++Library")
		FSlateColor SelectTextColor(const GameOverState state);
	UFUNCTION(BlueprintCallable, Category = "C++Library")
		ESlateVisibility GetGameOverTextVisibility();
	UFUNCTION(BlueprintCallable, Category = "C++Library")
		void SwitchGameOverStateAction();
	void NextSelectState();
	void BackSelectState();
	//�Ή�����I���X�e�[�g�̃A�N�V���������s����
	void SelectStateAction();
	//�Ăяo������BP�ŃA�j���[�V�����Đ�
	UFUNCTION(Category = "C++Event", BlueprintImplementableEvent, BlueprintCallable)
		void InitPlayAnimation();
	UFUNCTION(Category = "C++Event", BlueprintImplementableEvent, BlueprintCallable)
		void EndPlayAnimation();
};