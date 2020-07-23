// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleController.h"
#include "Kismet/GameplayStatics.h"
#include "GoalComponent.h"
#include "Blueprint/UserWidget.h"
#include "../UI/ControlTipsUI.h"
#include "../UI/BattleTimeUI.h"
#include "../UI/BattleResultUI.h"
#include "../UI/BattleGameStartUI.h"
#include "../InputManager.h"
#include "../Player/PlayerCharacter.h"
#include "../WaterSurface/FloatActor.h"
#include "TimerManager.h"
#include "../SoundManager.h"
#include "../Camera/GameCameraFocusPoint.h"
#include "../UI/FadeUI.h"
#include "../Camera/GameCameraActor.h"
#include "Components/MaterialBillboardComponent.h"
#include "../SoundManager.h"
#include "../WaterSurface/WaterSurface.h"
#include "Engine/GameViewportClient.h"

// Sets default values
ABattleController::ABattleController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABattleController::BeginPlay()
{
	Super::BeginPlay();

	// 画面分割有効
	GetWorld()->GetGameViewport()->SetForceDisableSplitscreen(false);

	// 各プレイヤーを取得
	TArray<class AActor*> FoundPlayers;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), FoundPlayers);
	for (auto Actor : FoundPlayers)
	{
		// 取得したアクターがプレイヤーかどうかを判定
		APlayerCharacter * Player = Cast<APlayerCharacter>(Actor);
		if (!Player) continue;

		// プレイヤー番号を取得して1,2それぞれに保存する
		if (Player->GetBattleNumber() == 1)
		{
			GetPlayer1 = Player;
			Player1RespawnLocation = Player->GetActorLocation();
		}
		else if (Player->GetBattleNumber() == 2)
		{
			GetPlayer2 = Player;
			Player2RespawnLocation = Player->GetActorLocation();
		}
	}

	IsStartResultEvent = false;

	//ポーズ中でもTickが来るようにする
	SetTickableWhenPaused(true);

	// デバッグスクリーンモードならプレイヤーやアイコンを非表示に
	if (DebugScreenMode)
	{
		GetPlayer1->SetPlayerHiddenInGame();
		GetPlayer1->DebugHammerCountBarParent();
		GetPlayer1->SetNoTick();
		GetPlayer2->SetPlayerHiddenInGame();
		GetPlayer2->DebugHammerCountBarParent();
		GetPlayer2->SetNoTick();
		SetAllInvisibleStageIcon();
		return;
	}

	// TODO::バトル用の時間のUIを作成
	//CreateGameTimeUI();

	// TODO::バトル用のUIに作成
	//GetPlayer1->CreateHammerCountBarUI();
	//GetPlayer2->CreateHammerCountBarUI();
	if (IsTips)
	{
		CreateControlTipsUI();
	}
	CreateBattleTimeUI();
	if (BattleTimeUI)
	{
		StartTimeCount();
	}
	//最後にフェードアウトを出すため最後に生成
	InitFadeOut();
	GetPlayer1->SetNoTickSingle(true);
	GetPlayer2->SetNoTickSingle(true);
	// BGMを再生
	AudioComponent = ASoundManager::CreateAudioComponent(SOUND_TYPE::STAGE_BGM);
	AudioComponent->Play(0.0f);
}

// Called every frame
void ABattleController::Tick(float DeltaTime)
{
	if (DebugScreenMode)return;
	if (FadeUI->GetFadeIsEnable())return;
	if (BattleGameStartUIClass != nullptr)
	{
		if (!FadeUI->GetFadeIsEnable() && !BattleGameStartUI)
		{
			//フェードアウト終了次第カウントダウンを開始するようにする
			CreateBattleGameStartUI();
		}
		if (BattleGameStartUI->GetIsCountDownZero() && GetPlayer1->GetNoTick())
		{
			GetPlayer1->SetNoTickSingle(false);
			GetPlayer2->SetNoTickSingle(false);
		}
	}
	Super::Tick(DeltaTime);

	CheckPlayerFall();
	
	UpdateTime();
	if (GetLimitTimeZero())
	{
		BattleFinish();
	}
	if (IsTimeOver)
	{
		CreateBattleResultUI();
		UGameplayStatics::SetGamePaused(GetWorld(), true);
	}
}

void ABattleController::InitFadeOut()
{
	if (FadeUI)return;
	if (FadeUIClass != nullptr)
	{
		FadeUI = CreateWidget<UFadeUI>(GetWorld(), FadeUIClass);
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

void ABattleController::CreateControlTipsUI()
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

void ABattleController::CreateBattleTimeUI()
{
	if (BattleTimeUI)return;
	if (BattleTimeUIClass != nullptr)
	{
		BattleTimeUI = CreateWidget<UBattleTimeUI>(GetWorld(), BattleTimeUIClass);
		if (BattleTimeUI != nullptr)
		{
			BattleTimeUI->AddToViewport();
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("BattleTimeUIClass : %s"), L"Widget cannot create");
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("BattleTimeUIClass : %s"), L"UIClass is nullptr");
	}
}

void ABattleController::CreateBattleResultUI()
{
	if (BattleResultUI)return;
	if (BattleResultUIClass != nullptr)
	{
		BattleResultUI = CreateWidget<UBattleResultUI>(GetWorld(), BattleResultUIClass);
		if (BattleResultUI != nullptr)
		{
			BattleResultUI->AddToViewport();
			BattleResultUI->SetPlayerScore(0, Player1Score);
			BattleResultUI->SetPlayerScore(1, Player2Score);
			if (Player1Score >= Player2Score)
			{
				BattleResultUI->SetWinPlayer(true);
			}
			else
			{
				BattleResultUI->SetWinPlayer(false);
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("BattleResultUIClass : %s"), L"Widget cannot create");
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("BattleResultUIClass : %s"), L"UIClass is nullptr");
	}
}

void ABattleController::CreateBattleGameStartUI()
{
	if (BattleGameStartUI)return;
	if (BattleGameStartUIClass != nullptr)
	{
		BattleGameStartUI = CreateWidget<UBattleGameStartUI>(GetWorld(), BattleGameStartUIClass);
		if (BattleGameStartUI != nullptr)
		{
			BattleGameStartUI->AddToViewport();
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("BattleGameStartUIClass : %s"), L"Widget cannot create");
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("BattleGameStartUIClass : %s"), L"UIClass is nullptr");
	}
}

void ABattleController::UpdateTime()
{
	//ゲームが終了していたら時計を進めない
	if (IsBatlleFinish)return;
	if (UGameplayStatics::IsGamePaused(GetWorld()))
	{
		SetTimeCountPause();
		return;
	}
	if (!UGameplayStatics::IsGamePaused(GetWorld()))
	{
		SetTimeCountRePlay();
	}

	// バトル用時間のUIの更新を行う
	//if (GameTimeUI)
	//{
	//	GameTimeUI->UpDateTime();
	//}
}

bool ABattleController::GetLimitTimeZero()
{
	// TODO::バトル用の時間のUIを参照して時間終了かどうかを判定する
	//if (GameTimeUI)
	//{
	//	//カウントダウンと針が0を指していたら時間終了とする
	//	if (GameTimeUI->GetIsCountZero() && GameTimeUI->GetIsTimeEnd())
	//	{
	//		return true;
	//	}
	//}
	return false;
}

void ABattleController::SetTimeCountPause()
{
	// TODO::バトル用の時間のUIを一時停止
	//if (GameTimeUI)
	//{
	//	GameTimeUI->AnimationPause();
	//}
}

void ABattleController::SetTimeCountRePlay()
{
	// TODO::バトル用の時間のUIを再開
	//if (GameTimeUI)
	//{
	//	GameTimeUI->AnimationRePlay();
	//}
}

float ABattleController::GetNowTimeAngle()
{
	// バトル用時間ＵＩから現在の針の角度を取得する
	//return GameTimeUI->GetNowTimeAngle();
	return 0;
}

void ABattleController::PauseCall()
{
	//アタックアニメ中はポーズを開けないようにする
	if (IsBatlleFinish)return;
	if (GetPlayer1->GetIsAttack())return;
	if (GetPlayer1->GetIsCharge())return;
	if (GetPlayer2->GetIsAttack())return;
	if (GetPlayer2->GetIsCharge())return;
	IsPause = true;
	if (!UGameplayStatics::IsGamePaused(GetWorld()))
	{
		// TODO::バトル用ポーズUIを作成して開く
	}
	else if (UGameplayStatics::IsGamePaused(GetWorld()))
	{
		if (!PauseUI)return;
		IsPause = false;
		// TODO::バトル用ポーズUIを閉じる
	}
}

void ABattleController::InputRightCall()
{
	// TODO::ポーズとかリザルトのUIができたら、そこの操作を記述
}

void ABattleController::InputLeftCall()
{
	// TODO::ポーズとかリザルトのUIができたら、そこの操作を記述
}

void ABattleController::InputSelectCall()
{
	// TODO::ポーズとかリザルトのUIができたら、そこの操作を記述
}

void ABattleController::StopBGM()
{
	if (!AudioComponent) return;
	if (!(AudioComponent->IsPlaying())) AudioComponent->Stop();
}

void ABattleController::RemoveUI()
{
	// TODO::バトル用時間UIを非表示にする

	if (ControlTipsUI)
	{
		ControlTipsUI->RemoveFromParent();
	}
	GetPlayer1->HammerCountBarParent();
	GetPlayer2->HammerCountBarParent();
}

void ABattleController::SetAllInvisibleStageIcon()
{
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), Actors);

	for (auto Actor : Actors)
	{
		TArray<UActorComponent*> Billboards;
		Actor->GetComponents(UMaterialBillboardComponent::StaticClass(), Billboards);

		for (auto Billboard : Billboards)
		{
			Cast<UMaterialBillboardComponent>(Billboard)->SetRenderInMainPass(false);
		}
	}
}

void ABattleController::BattleFinish()
{
	// TODO::勝敗の判定をしてバトル用リザルトUIを開く

	IsBatlleFinish = true;
}

void ABattleController::CheckPlayerFall()
{
	if (!GetPlayer1)return;
	if (GetPlayer1->GetIsDeth())
	{
		// いかだとプレイヤーをリスポーンさせて相手のスコアを加算
		GetPlayer1->PlayerRespawn(Player1RespawnLocation);
		AActor* a = GetWorld()->SpawnActor<AActor>(RaftActor); 
		FVector RaftLocation = Player1RespawnLocation;
		RaftLocation.Z = 0;
		a->SetActorLocation(RaftLocation);
		Player2Score += 1;
	}

	if (!GetPlayer2)return;
	if (GetPlayer2->GetIsDeth())
	{
		GetPlayer2->PlayerRespawn(Player2RespawnLocation);
		AActor* a = GetWorld()->SpawnActor<AActor>(RaftActor);
		FVector RaftLocation = Player2RespawnLocation;
		RaftLocation.Z = 0;
		a->SetActorLocation(RaftLocation);
		Player1Score += 1;
	}
}