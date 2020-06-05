// Fill out your copyright notice in the Description page of Project Settings.

#include "GameController.h"
#include "Kismet/GameplayStatics.h"
#include "Goal.h"
#include "Blueprint/UserWidget.h"
#include "../UI/TimeCountUI.h"
#include "../UI/GameOverUI.h"
#include "../UI/ResultUI.h"
#include "../InputManager.h"
#include "../Player/PlayerCharacter.h"
#include "../WaterSurface/FloatActor.h"
#include "TimerManager.h"
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
	GetPlayer = Cast<APlayerCharacter>(UGameplayStatics::GetActorOfClass(GetWorld(), APlayerCharacter::StaticClass()));
	IsGameClear = false;
	IsGameOver = false;
	DataTableLoad();
	// シーン上のゴールを全て取得
	TArray<class AActor*> FoundGoals;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGoal::StaticClass(), FoundGoals);
	for (auto Actor : FoundGoals)
	{
		GoalArray.Add(Cast<AGoal>(Actor));
	}
	SetNorma();
	//ポーズ中でもTickが来るようにする
	SetTickableWhenPaused(true);
	//MaxNimotuが0の時はデバッグモードなので現在のマップの荷物数を得る
	if (MaxNimotu == 0)
	{
		GetMaxNimotu();
	}
	GameMaxNimotu = MaxNimotu;
	NotExplotionCount = GetNotExplotionCount(); //壊れていない家の数Get
}

// Called every frame
void AGameController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//プレイヤーへのアドレスを取得出来ていなかったら再び貰う
	if (!GetPlayer)
	{
		GetPlayer = Cast<APlayerCharacter>(UGameplayStatics::GetActorOfClass(GetWorld(), APlayerCharacter::StaticClass()));
	}
	CheckTimeCount();
	GameClearCheck();
	GameOverCheck();
	InputGameOverUI();
	InputResultUI();
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

void AGameController::CreateTimeCountUI()
{
	if (TimeCountUI)return;
	if (TimeCountUIClass != nullptr)
	{
		TimeCountUI = CreateWidget<UTimeCountUI>(GetWorld(), TimeCountUIClass);
		if (TimeCountUI != nullptr)
		{
			TimeCountUI->AddToViewport();
			TimeCountUI->SetTimeCount(LimitTime);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("TimeCountUIClass : %s"), L"Widget cannot create");
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("TimeCountUIClass : %s"), L"UIClass is nullptr");
	}
}

void AGameController::CreateGameOverUI()
{
	if (GameOverUI)return;
	if (GameOverUIClass != nullptr)
	{
		GameOverUI = CreateWidget<UGameOverUI>(GetWorld(), GameOverUIClass);
		if (GameOverUI != nullptr)
		{
			GameOverUI->AddToViewport();
			GetPlayer->SetNoTick();
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("GameOverUIClass : %s"), L"Widget cannot create");
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("GameOverUIClass : %s"), L"UIClass is nullptr");
	}
}

void AGameController::CreateResultUI()
{
	if (ResultUI)return;
	if (ResultUIClass != nullptr)
	{
		ResultUI = CreateWidget<UResultUI>(GetWorld(), ResultUIClass);
		if (ResultUI != nullptr)
		{
			ResultUI->AddToViewport();
			GetPlayer->HammerCountBarParent();
			ResultUI->SetResultGaugeAnimeCheckEvent(GetPlayer->GetMaxHammerHP(),GetPlayer->GetHammerHP(), NormaPercent);
			//ResultUI->SetResultGaugeAnimeCheckEvent(100.0f, 80.0f, 30.0f);
			ResultUI->SetResultNowNimotuCheckEvent(GoalCount);
			//ResultUI->SetResultNowNimotuCheckEvent(3);
			ResultUI->SetStampAnimeCheckEvent(NormaGoalCount, MaxNimotu);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("ResultUIClass : %s"), L"Widget cannot create");
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ResultUIClass : %s"), L"UIClass is nullptr");
	}
}

void AGameController::InputGameOverUI()
{
	if (!IsGameOver)return;
	//if (!UGameplayStatics::IsGamePaused(GetWorld()))return;
	if (!GameOverUI)return;
	if (GameOverUI->GetIsPlayAnimation())return;
	const AInputManager * inputManager = AInputManager::GetInstance();
	if (!inputManager)return;
	const InputState * input = inputManager->GetState();
	if (input->Up.IsPress)
	{
		GameOverUI->BackSelectState();
	}
	if (input->Down.IsPress)
	{
		GameOverUI->NextSelectState();
	}
	if (input->Select.IsPress)
	{
		GameOverUI->SelectStateAction();
	}
}

void AGameController::InputResultUI()
{
	if (!IsGameClear)return;
	if (!ResultUI)return;
	const AInputManager * inputManager = AInputManager::GetInstance();
	if (!inputManager)return;
	const InputState * input = inputManager->GetState();
	if (input->Up.IsPress)
	{
		ResultUI->BackSelectState();
	}
	if (input->Down.IsPress)
	{
		ResultUI->NextSelectState();
	}
	if (input->Select.IsPress)
	{
		ResultUI->SelectStateAction();
	}
}

void AGameController::SetNorma()
{
}

int AGameController::GetMaxNimotu()
{
	//すでに荷物数を数えているならリターン
	if (MaxNimotu != 0)return MaxNimotu;
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFloatActor::StaticClass(), FoundActors);
	for (auto Actor : FoundActors)
	{
		AFloatActor* act = Cast<AFloatActor>(Actor);
		if (act)
		{
			if (act->ActorHasTag("Nimotu"))
			{
				MaxNimotu++;
			}
		}
	}
	return MaxNimotu;
}

void AGameController::GameClearCheck()
{
	if (IsGameOver)return;
	auto gameclear = [=]
	{
		//指定の時間後ゲームクリアにする
		IsGameClear = true;
		FTimerManager& timerManager = GetWorld()->GetTimerManager();
		FTimerHandle handle;
		timerManager.SetTimer(handle, this, &AGameController::GameClear, 2.8f);
	};
	// ゲームクリア条件
	//①ノルマ以上荷物を入れている時かつハンマーが壊れて残り時間が0になったら
	if (GoalCount >= NormaGoalCount && GetLimitTimeZero())
	{
		gameclear();
	}
	//②荷物を全て入れる
	else if (GoalCount == MaxNimotu)
	{
		gameclear();
	}
	//③ゲーム内の荷物が0でゴールカウントがノルマより大きかったら
	else if (GameMaxNimotu == 0 &&  GoalCount >= NormaGoalCount)
	{
		gameclear();
	}
}

void AGameController::GameOverCheck()
{
	if (IsGameClear)return;
	// ゲームオーバー条件
	//ノルマを1つも達成できなくなったらゲームオーバー
	auto gameover = [=] 
	{ 
		IsGameOver = true;
		//指定の時間後ゲームオーバーにする
		FTimerManager& timerManager = GetWorld()->GetTimerManager();
		FTimerHandle handle;
		timerManager.SetTimer(handle, this, &AGameController::GameOver,3.0f);
	};
	//①ノルマまで荷物を運んでおらずハンマーが壊れて残り時間が0になったら
	if (GoalCount < NormaGoalCount && GetLimitTimeZero())
	{
		gameover();
	}
	//②荷物がノルマ数達成できないほど無くなった時(ゴールに入った荷物と合わせる)
	else if (GameMaxNimotu + GoalCount < NormaGoalCount)
	{//このゲームオーバーの時はTimerを使わない
		IsGameOver = true;
		GameOver();
	}
	//③ゴールがノルマの荷物より少なくなった時
	else if (NotExplotionCount < NormaGoalCount && GoalCount < NormaGoalCount)
	{
		gameover();
	}
}

void AGameController::GameClear()
{
	CreateResultUI();
	if (TimeCountUI)
	{
		TimeCountUI->RemoveFromParent();
	}
}

void AGameController::GameOver()
{
	CreateGameOverUI();
	if (TimeCountUI)
	{
		TimeCountUI->RemoveFromParent();
	}
}

int AGameController::CountGameNimotu()
{
	int num = 0;
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFloatActor::StaticClass(), FoundActors);
	for (auto Actor : FoundActors)
	{
		AFloatActor* act = Cast<AFloatActor>(Actor);
		if (act)
		{
			if (act->ActorHasTag("Nimotu"))
			{
				num++;
			}
		}
	}
	return num;
}

void AGameController::CheckTimeCount()
{

}

bool AGameController::GetLimitTimeZero()
{
	if (TimeCountUI)
	{
		return TimeCountUI->GetIsCountZero();
	}
	return false;
}

void AGameController::SetTimeCountPause()
{
	if (TimeCountUI)
	{
		TimeCountUI->AnimationPause();
	}
}

void AGameController::SetTimeCountRePlay()
{
	if (TimeCountUI)
	{
		TimeCountUI->AnimationRePlay();
	}
}

bool AGameController::GetIsGameOverUI()
{
	if (GameOverUI)
	{
		return true;
	}
	return false;
}