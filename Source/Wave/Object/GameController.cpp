// Fill out your copyright notice in the Description page of Project Settings.

#include "GameController.h"
#include "Kismet/GameplayStatics.h"
#include "GoalComponent.h"
#include "Blueprint/UserWidget.h"
#include "../UI/GameTimeUI.h"
#include "../UI/GameOverUI.h"
#include "../UI/ResultUI.h"
#include "../UI/NimotuCountUI.h"
#include "../UI/PauseUI.h"
#include "../UI/ControlTipsUI.h"
#include "../InputManager.h"
#include "../Player/PlayerCharacter.h"
#include "../WaterSurface/FloatActor.h"
#include "TimerManager.h"
#include "../SoundManager.h"
#include "../Camera/GameCameraFocusPoint.h"
#include "../UI/FadeUI.h"
#include "../Camera/GameCameraActor.h"
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
	IsStartResultEvent = false;
	NotExplotionCount = 0;
	DataTableLoad();

	//ポーズ中でもTickが来るようにする
	SetTickableWhenPaused(true);

	// シーン上のゴールを全て取得
	TArray<class AActor*> FoundGoals;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), FoundGoals);
	for (auto Actor : FoundGoals)
	{
		UGoalComponent * Goal = Cast<UGoalComponent>(Actor->GetComponentByClass(UGoalComponent::StaticClass()));
		if (!Goal) continue;

		if(!Goal->GetIsExplotion())
			NotExplotionCount++;
	}

	//MaxNimotuが0の時はデバッグモードなので現在のマップの荷物数を得る
	if (MaxNimotu == 0)
	{
		GetMaxNimotu();
	}
	GameMaxNimotu = MaxNimotu;
	if (DebugScreenMode)
	{
		GetPlayer->SetPlayerHiddenInGame();
		GetPlayer->DebugHammerCountBarParent();
		GetPlayer->SetNoTick();
		return;
	}
	CreateNimotuCountUI();
	CreateGameTimeUI();
	GetPlayer->CreateHammerCountBarUI();
	if (IsTips)
	{
		CreateControlTipsUI();
	}
	//最後にフェードアウトを出すため最後に生成
	InitFadeOut();

	AudioComponent = ASoundManager::CreateAudioComponent(SOUND_TYPE::STAGE_BGM);
	AudioComponent->Play(0.0f);
}

// Called every frame
void AGameController::Tick(float DeltaTime)
{
	if (DebugScreenMode)return;
	Super::Tick(DeltaTime);
	//プレイヤーへのアドレスを取得出来ていなかったら再び貰う
	if (!GetPlayer)
	{
		GetPlayer = Cast<APlayerCharacter>(UGameplayStatics::GetActorOfClass(GetWorld(), APlayerCharacter::StaticClass()));
	}
	////アタックアニメ中はポーズを開けないようにする
	//if (!GetPlayer->GetIsAttack() && !GetPlayer->GetIsCharge())
	//{
	////	InputPause();
	//}

	//カメラの注目ポイントが存在する場合はゲームの判定をしない
	if (!IsStartResultEvent && GetPlayer->GetCameraActor()->FocusPoints.Num() == 0)
	{
		//指定の時間後に関数を実行する
		// ゲームオーバーを優先して判定する（全てゴール後に家が落ちたり壊れたりするため）
		if (IsGameOver)
		{
			FTimerManager& timerManager = GetWorld()->GetTimerManager();
			FTimerHandle handle;
			timerManager.SetTimer(handle, this, &AGameController::GameOver, 1.0f);
			IsStartResultEvent = true;
		}
		else if (IsGameClear)
		{
			GetPlayer->SetGameClear();
		    ASoundManager::GetInstance()->PlaySound(SOUND_TYPE::CLEAR_BGM);
			FTimerManager& timerManager = GetWorld()->GetTimerManager();
			FTimerHandle handle;
			timerManager.SetTimer(handle, this, &AGameController::GameClear, 2.8f);
			IsStartResultEvent = true;
		}
	}

	GameOverCheck();
	GameClearCheck();

	//InputGameOverUI();
	UpdateTime();
}

void AGameController::CreateGameTimeUI()
{
	if (GameTimeUI)return;
	if (GameTimeUIClass != nullptr)
	{
		GameTimeUI = CreateWidget<UGameTimeUI>(GetWorld(), GameTimeUIClass);
		if (GameTimeUI != nullptr)
		{
			GameTimeUI->AddToViewport();
			GameTimeUI->SetTimeLimit(TimeLimit);
			GameTimeUI->SetCountDownTime(CountDownTime);
			GameTimeUI->SetNormaTime(NormaTime);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("GameTimeUIClass : %s"), L"Widget cannot create");
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("GameTimeUIClass : %s"), L"UIClass is nullptr");
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
			//このステージをクリアした時の制限時間、クリア時の針アングル、ノルマ針アングル、ノルマ時間をセット
			ResultUI->SetResultTokeiAnimeCheckEvent(TimeLimit, GetNowTimeAngle(), GetNormaTimeAngle(),NormaTime);
			ResultUI->SetResultNowNimotuCheckEvent(GoalCount);
			ResultUI->SetStampAnimeCheckEvent(NormaGoalCount, MaxNimotu);
			SetTimeCountPause();
			IsResult = true;
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

void AGameController::CreateNimotuCountUI()
{
	if (NimotuCountUI)return;
	if (NimotuCountUIClass != nullptr)
	{
		NimotuCountUI = CreateWidget<UNimotuCountUI>(GetWorld(), NimotuCountUIClass);
		if (NimotuCountUI != nullptr)
		{
			NimotuCountUI->AddToViewport();
			NimotuCountUI->SetStageNimotu(GameMaxNimotu);
			NimotuCountUI->SetStageInNimotu(GoalCount);
			NimotuCountUI->SetNormaNimotu(NormaGoalCount);
			NimotuCountUI->SetMaxNimotu(MaxNimotu);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("NimotuCountUIClass : %s"), L"Widget cannot create");
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("NimotuCountUIClass : %s"), L"UIClass is nullptr");
	}
}

void AGameController::InitFadeOut()
{
	if (FadeUIClass != nullptr)
	{
		UFadeUI* FadeUI = CreateWidget<UFadeUI>(GetWorld(), FadeUIClass);
		if (FadeUI != nullptr)
		{
			FadeUI->AddToViewport();
			FadeUI->SetFade(FLinearColor(0.0f, 0.0f, 0.0f, 1.0f), false, 1.0f, true);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("FadeUIClass : %s"), L"Widget cannot create");
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("FadeUIClass : %s"), L"UIClass is nullptr");
	}
}

void AGameController::InputGameOverUI()
{
	if (!IsGameOver)return;
	if (!GameOverUI)return;
	if (GameOverUI->GetIsPlayAnimation())return;
	const AInputManager * inputManager = AInputManager::GetInstance();
	if (!inputManager)return;
	const InputState * input = inputManager->GetState();
	if (input->Left.IsPress)
	{
		GameOverUI->BackSelectState();
	}
	if (input->Right.IsPress)
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
	if (input->Left.IsPress)
	{
		ResultUI->BackSelectState();
	}
	if (input->Right.IsPress)
	{
		ResultUI->NextSelectState();
	}
	if (input->Select.IsPress)
	{
		ResultUI->SelectStateAction();
	}
}

void AGameController::InputPause()
{
	const AInputManager * inputManager = AInputManager::GetInstance();
	if (!inputManager)return;
	const InputState * input = inputManager->GetState();
	if (input->Pause.IsPress)
	{//ポーズ中でなければポーズ画面を開き、ポーズ中だったらポーズ画面を閉じる
		//ゲーム終了条件を満たしていたらポーズを開けないようにする
		if (GetIsClear() || GetIsGameOver())return;
		if (!UGameplayStatics::IsGamePaused(GetWorld()))
		{
			if (PauseUIClass != nullptr)
			{//初めてポーズ画面を開くときはUIを生成する
				if (!PauseUI)
				{
					PauseUI = CreateWidget<UPauseUI>(GetWorld(), PauseUIClass);
					PauseUI->AddToViewport();
					PauseUI->SetNormaAngle(GetNormaTimeAngle());
					PauseUI->SetNeedleAndBG_Material(GetNowTimeAngle());
					PauseUI->SetTimeLimit(TimeLimit);
					PauseUI->SetNormaTime(TimeLimit - NormaTime);
					SetTimeCountPause();
				}
				else if (PauseUI)
				{
					if (PauseUI->GetIsPlayAnimation())return;
					PauseUI->AddToViewport();
					PauseUI->SetNormaAngle(GetNormaTimeAngle());
					PauseUI->SetNeedleAndBG_Material(GetNowTimeAngle());
					PauseUI->SetTimeLimit(TimeLimit);
					PauseUI->SetNormaTime(TimeLimit - NormaTime);
					SetTimeCountPause();
				}
				//生成してもnullptrだったらエラー文表示
				if (PauseUI == nullptr)
				{
					UE_LOG(LogTemp, Error, TEXT("PauseUI : %s"), L"Widget cannot create");
				}
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("PauseUI : %s"), L"PauseUIClass is nullptr");
			}
		}
		else if (UGameplayStatics::IsGamePaused(GetWorld()))
		{
			if (!PauseUI)return;
			if (PauseUI->GetIsPlayAnimation())return;
			PauseUI->EndAnimation();
		}
	}
	if (!UGameplayStatics::IsGamePaused(GetWorld()))return;
	if (!PauseUI)return;
	if (input->Left.IsPress)
	{
		PauseUI->BackSelectState();
	}
	if (input->Right.IsPress)
	{
		PauseUI->NextSelectState();
	}
	if (input->Select.IsPress)
	{
		PauseUI->SelectStateAction();
	}
}

void AGameController::CreateControlTipsUI()
{
	if (ControlTipsUI)return;
	if (ControlTipsUIClass != nullptr)
	{
		ControlTipsUI = CreateWidget<UControlTipsUI>(GetWorld(), ControlTipsUIClass);
		if (ControlTipsUI != nullptr)
		{
			ControlTipsUI->AddToViewport();
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("ControlTipsUIClass : %s"), L"Widget cannot create");
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ControlTipsUIClass : %s"), L"UIClass is nullptr");
	}
}

void AGameController::UpdateTime()
{
	//ゲームが終了していたら時計を進めない
	if (GetIsClear() || GetIsGameOver())return;
	if (UGameplayStatics::IsGamePaused(GetWorld()))
	{
		SetTimeCountPause();
		return;
	}
	if (!UGameplayStatics::IsGamePaused(GetWorld()))
	{
		SetTimeCountRePlay();
	}
	if (GameTimeUI)
	{
		GameTimeUI->UpDateTime();
	}
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
	//if (IsGameOver)return;
	if (IsGameClear)return;
	auto gameclear = [=]
	{
		RemoveUI();
		//指定の時間後ゲームクリアにする
		IsGameClear = true;
		SetTimeCountPause();
		AudioComponent->FadeOut(1.0f, 0.0f);
	};
	// ゲームクリア条件
	//①ノルマ以上荷物を入れている時かつ残り時間が0になったら
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
	//④ゴールが全て入っているかつ荷物が残っているかつノルマ以上にゴールに入れていたらクリア
	else if (NotExplotionCount - GoalCount <= 0 && GameMaxNimotu > 0 && GoalCount >= NormaGoalCount)
	{
		gameclear();
	}
}

void AGameController::GameOverCheck()
{
	//if (IsGameClear)return;
	if (IsGameOver)return;
	// ゲームオーバー条件
	//ノルマを1つも達成できなくなったらゲームオーバー
	auto gameover = [=]
	{ 
		RemoveUI();
		IsGameOver = true;
		SetTimeCountPause();
		AudioComponent->FadeOut(2.0f, 0.0f);
	};
	//①ノルマまで荷物を運んでおらず残り時間が0になったら
	if (GoalCount < NormaGoalCount && GetLimitTimeZero())
	{
		gameover();
	}
	//②プレイヤーが落下した時
	else if (GetPlayer->GetIsDeth())
	{
		gameover();
	}
	//③荷物がノルマ数達成できないほど無くなった時(ゴールに入った荷物と合わせる)
	else if (GameMaxNimotu + GoalCount < NormaGoalCount)
	{
		gameover();
	}
	//④ゴールがノルマの荷物より少なくなった時
	else if (NotExplotionCount < NormaGoalCount && GoalCount - NotExplotionCount < NormaGoalCount)
	{
		gameover();
	}
}

void AGameController::GameClear()
{
	CreateResultUI();
}

void AGameController::GameOver()
{
	CreateGameOverUI();
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

bool AGameController::GetLimitTimeZero()
{
	if (GameTimeUI)
	{
		return GameTimeUI->GetIsCountZero();
	}
	return false;
}

void AGameController::SetTimeCountPause()
{
	if (GameTimeUI)
	{
		GameTimeUI->AnimationPause();
	}
}

void AGameController::SetTimeCountRePlay()
{
	if (GameTimeUI)
	{
		GameTimeUI->AnimationRePlay();
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

void AGameController::AddGoalCount()
{
	GoalCount++; 
	//現在ノルマと同じ数を入れていたらアニメーション再生
	if (NormaGoalCount == GoalCount)
	{
		NimotuCountUI->NormaInAnimation();
	}
	NimotuCountUI->NimotuInAnimation();
	NimotuCountUI->SetStageInNimotu(GoalCount);
}

void AGameController::MinusGoalCount()
{ 
	//現在ノルマと同じ数を入れていたらノルマ以下の荷物になったことを示すアニメーション再生
	if (NormaGoalCount == GoalCount)
	{
		NimotuCountUI->NormaNoAnimation();
	}
	GoalCount--;
	NimotuCountUI->NimotuInAnimation();
	NimotuCountUI->SetStageInNimotu(GoalCount);
}

void AGameController::MinusGameMaxNimotu()
{ 
	GameMaxNimotu--;
	NimotuCountUI->SetStageNimotu(GameMaxNimotu);
	NimotuCountUI->GameNimotuAnimation();
}

float AGameController::GetNowTimeAngle()
{
	return GameTimeUI->GetNowTimeAngle();
}

float AGameController::GetNormaTimeAngle()
{
	return GameTimeUI->GetNormaTimeAngle();
}

void AGameController::PauseCall()
{
	//アタックアニメ中はポーズを開けないようにする
	if (IsGameClear)return;
	if (IsGameOver)return;
	if (GetPlayer->GetIsAttack())return;
	if (GetPlayer->GetIsCharge())return;
	IsPause = true;
	if (!UGameplayStatics::IsGamePaused(GetWorld()))
	{
		if (PauseUIClass != nullptr)
		{//初めてポーズ画面を開くときはUIを生成する
			if (!PauseUI)
			{
				PauseUI = CreateWidget<UPauseUI>(GetWorld(), PauseUIClass);
				PauseUI->AddToViewport();
				PauseUI->SetNormaAngle(GetNormaTimeAngle());
				PauseUI->SetNeedleAndBG_Material(GetNowTimeAngle());
				PauseUI->SetTimeLimit(TimeLimit);
				PauseUI->SetNormaTime(TimeLimit - NormaTime);
				SetTimeCountPause();
			}
			else if (PauseUI)
			{
				if (PauseUI->GetIsPlayAnimation())return;
				PauseUI->AddToViewport();
				PauseUI->SetNormaAngle(GetNormaTimeAngle());
				PauseUI->SetNeedleAndBG_Material(GetNowTimeAngle());
				PauseUI->SetTimeLimit(TimeLimit);
				PauseUI->SetNormaTime(TimeLimit - NormaTime);
				SetTimeCountPause();
			}
			//生成してもnullptrだったらエラー文表示
			if (PauseUI == nullptr)
			{
				UE_LOG(LogTemp, Error, TEXT("PauseUI : %s"), L"Widget cannot create");
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("PauseUI : %s"), L"PauseUIClass is nullptr");
		}
	}
	else if (UGameplayStatics::IsGamePaused(GetWorld()))
	{
		if (!PauseUI)return;
		if (PauseUI->GetIsPlayAnimation())return;
		PauseUI->EndAnimation();
		IsPause = false;
	}
}

void AGameController::InputRightCall()
{
	if (IsGameOver)
	{
		if (!GameOverUI)return;
		if (GameOverUI->GetIsPlayAnimation())return;
		GameOverUI->NextSelectState();
	}
	if (!UGameplayStatics::IsGamePaused(GetWorld()))return;
	if (IsPause)
	{
		if (!PauseUI)return;
		PauseUI->NextSelectState();
	}
	else if (IsResult)
	{
		if (!IsGameClear)return;
		if (!ResultUI)return;
		ResultUI->NextSelectState();
	}
}

void AGameController::InputLeftCall()
{
	if (IsGameOver)
	{
		if (!GameOverUI)return;
		if (GameOverUI->GetIsPlayAnimation())return;
		GameOverUI->BackSelectState();
	}
	if (!UGameplayStatics::IsGamePaused(GetWorld()))return;
	if (IsPause)
	{
		if (!PauseUI)return;
		PauseUI->BackSelectState();
	}
	else if (IsResult)
	{
		if (!IsGameClear)return;
		if (!ResultUI)return;
		ResultUI->BackSelectState();
	}
}

void AGameController::InputSelectCall()
{
	if (IsGameOver)
	{
		if (!GameOverUI)return;
		if (GameOverUI->GetIsPlayAnimation())return;
		GameOverUI->SelectStateAction();
	}
	if (!UGameplayStatics::IsGamePaused(GetWorld()))return;
	if (IsPause)
	{
		if (!PauseUI)return;
		PauseUI->SelectStateAction();
		IsPause = false;
	}
	else if (IsResult)
	{
		if (!IsGameClear)return;
		if (!ResultUI)return;
		ResultUI->SelectStateAction();
	}
}

void AGameController::RemoveUI()
{
	if (GameTimeUI)
	{
		GameTimeUI->RemoveFromParent();
	}
	if (NimotuCountUI)
	{
		NimotuCountUI->RemoveFromParent();
	}
	if (ControlTipsUI)
	{
		ControlTipsUI->RemoveFromParent();
	}
	GetPlayer->HammerCountBarParent();
}