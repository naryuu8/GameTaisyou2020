// Fill out your copyright notice in the Description page of Project Settings.

#include "GameController.h"
#include "Kismet/GameplayStatics.h"
#include "Goal.h"
#include "Blueprint/UserWidget.h"
#include "../UI/HammerCountUI.h"
#include "../UI/StageClearUI.h"
// Sets default values
AGameController::AGameController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGameController::BeginPlay()
{
	Super::BeginPlay();
	
	IsGameClear = false;

	// シーン上のゴールを全て取得
	TArray<class AActor*> FoundGoals;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGoal::StaticClass(), FoundGoals);
	for (auto Actor : FoundGoals)
	{
		GoalArray.Add(Cast<AGoal>(Actor));
	}

	CreateHammerCountUI();
}

// Called every frame
void AGameController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	int GoalCount = GetGoalCount();
	int NotExplotionCount = GetNotExplotionCount(); //壊れていない家の数Get
	//どうやってもここからノルマ達成は不可
	if (NotExplotionCount < NormaGoalCount)
	{
		IsGameOver = true;
		//下記あとゲームオーバーUIオナシャス
	}

	// ノルマ達成ならとりあえずゲームクリア
	if (GoalCount >= NormaGoalCount)
	{
		IsGameClear = true;
		CreateStageClearUI();
	}
}

int AGameController::GetGoalCount()
{
	int Count = 0;
	for (auto Goal : GoalArray)
	{
		if (Goal->GetIsGoal())
			Count++;
	}
	return Count;
}

int AGameController::GetNotExplotionCount()
{
	int Count = 0;
	for (auto Goal : GoalArray)
	{
		if (!Goal->GetIsExplotion())
			Count++;
	}
	return Count;
}


void AGameController::CreateHammerCountUI()
{
	//ハンマーカウントが0なら生成しない
	if (HammerCount == 0)return;
	if (UIClass != nullptr)
	{
		HammerCountUI = CreateWidget<UHammerCountUI>(GetWorld(), UIClass);
		if (HammerCountUI != nullptr)
		{
			HammerCountUI->AddToViewport();
			HammerCountUI->SetHammerCount(HammerCount);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("HammerCountUI : %s"), L"Widget cannot create");
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("HammerCountUI : %s"), L"UIClass is nullptr");
	}
}

void AGameController::CreateStageClearUI()
{
	if (StageClearUIClass != nullptr)
	{
		StageClearUI = CreateWidget<UStageClearUI>(GetWorld(), StageClearUIClass);
		if (HammerCountUI != nullptr)
		{
			StageClearUI->AddToViewport();
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("StageClearUIClass : %s"), L"Widget cannot create");
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("StageClearUIClass : %s"), L"UIClass is nullptr");
	}
}

void AGameController::MinusHammerCount()
{
	if (HammerCount == 0)return;
	if (!HammerCountUI)return;
	HammerCountUI->MinusHammerCount();
	HammerCountUI->MinusCountAnimation();
}