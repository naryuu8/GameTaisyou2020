// Fill out your copyright notice in the Description page of Project Settings.


#include "VersusController.h"
#include "Kismet/GameplayStatics.h"
#include "../Wave/Object/GoalComponent.h"
#include "Blueprint/UserWidget.h"
#include "../Wave/UI/ControlTipsUI.h"
#include "../Wave/UI/BattleTimeUI.h"
#include "../Wave/UI/BattleResultUI.h"
#include "../Wave/UI/BattleGameStartUI.h"
#include "../Wave/Player/PlayerCharacter.h"
#include "../Wave/WaterSurface/FloatActor.h"
#include "TimerManager.h"
#include "../Wave/SoundManager.h"
#include "../Wave/Camera/GameCameraFocusPoint.h"
#include "../Wave/UI/FadeUI.h"
#include "../Wave/Camera/GameCameraActor.h"
#include "Components/MaterialBillboardComponent.h"
#include "../Wave/SoundManager.h"
#include "../Wave/WaterSurface/WaterSurface.h"
#include "Engine/GameViewportClient.h"
#include "SectionController.h"
#include "WaterSurface/WaterSurface.h"
#include "UI/BattleResultUI.h"
#include "InputManager.h"
#include "../Wave/Camera/GameCameraActor.h"
// Sets default values
AVersusController::AVersusController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AVersusController::BeginPlay()
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
	// GetPlayer1->CreateHammerCountBarUI();
	// GetPlayer2->CreateHammerCountBarUI();
	if (IsTips)
	{
		CreateControlTipsUI();
	}
	//最後にフェードアウトを出すため最後に生成
	InitFadeOut();
	GetPlayer1->SetNoTickSingle(true);
	GetPlayer2->SetNoTickSingle(true);
	// BGMを再生
	AudioComponent = ASoundManager::CreateAudioComponent(SOUND_TYPE::VERSUS);
	AudioComponent->Play(0.0f);

	GetPlayer1->GetCameraActor()->Type = AGameCameraActor::FollowType::CharacterFollow_Far;
	GetPlayer2->GetCameraActor()->Type = AGameCameraActor::FollowType::CharacterFollow_Far;
}

void AVersusController::CreateSections()
{
	for (int i = 0; i < SectionCount; i++)
	{
		int index = FMath::RandRange(0, Sections.Num() - 1);
		AddSection(index, 1, i);
		AddSection(index, 2, i);
	}
}

void AVersusController::AddSection(int index, int PlayerNumber, int Cnt)
{
	FVector Location = FVector::ZeroVector;
	Location.X = SectionSize * Cnt;
	Location.Y = 1500 * (PlayerNumber - 1);
	ASectionController* section = GetWorld()->SpawnActor<ASectionController>(Sections[index]);
	section->SetActorLocation(Location);
	(PlayerNumber == 1 ? Player1Sections : Player2Sections).Add(section);
}

// Called every frame
void AVersusController::Tick(float DeltaTime)
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

	if (IsBatlleFinish)InputBattleResultUI();

	//if (IsTimeOver)
	//{
	//	CreateBattleResultUI();
	//	UGameplayStatics::SetGamePaused(GetWorld(), true);
	//}

	//UpdateTime();
	//if (GetLimitTimeZero())
	//{
	//	//　どうしようか
	//}
	//if (IsTimeOver)
	//{
	//	CreateBattleResultUI();
	//	UGameplayStatics::SetGamePaused(GetWorld(), true);
	//}
}

void AVersusController::InputBattleResultUI()
{
	if (!BattleResultUI)return;
	if (BattleResultUI->GetIsNoInput())return;
	const AInputManager * inputManager = AInputManager::GetInstance();
	if (!inputManager)return;

	const InputState * input = inputManager->GetState();
	if (input->Right.IsPress)
	{
		BattleResultUI->NextSelectState();
	}
	if (input->Left.IsPress)
	{
		BattleResultUI->BackSelectState();
	}
	if (input->Select.IsPress)
	{
		BattleResultUI->SelectStateAction();
	}
}

void AVersusController::InitFadeOut()
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

void AVersusController::CreateControlTipsUI()
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

void AVersusController::CreateBattleResultUI()
{
	if (BattleResultUI)return;
	if (BattleResultUIClass != nullptr)
	{
		BattleResultUI = CreateWidget<UBattleResultUI>(GetWorld(), BattleResultUIClass);
		if (BattleResultUI != nullptr)
		{
			BattleResultUI->AddToViewport();
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

void AVersusController::CreateBattleGameStartUI()
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

void AVersusController::InputRightCall()
{
	// TODO::ポーズとかリザルトのUIができたら、そこの操作を記述
}

void AVersusController::InputLeftCall()
{
	// TODO::ポーズとかリザルトのUIができたら、そこの操作を記述
}

void AVersusController::InputSelectCall()
{
	// TODO::ポーズとかリザルトのUIができたら、そこの操作を記述
}

void AVersusController::StopBGM()
{
	if (!AudioComponent) return;
	if (!(AudioComponent->IsPlaying())) AudioComponent->Stop();
}

void AVersusController::RemoveUI()
{
	// TODO::バトル用時間UIを非表示にする

	if (ControlTipsUI)
	{
		ControlTipsUI->RemoveFromParent();
	}
	GetPlayer1->HammerCountBarParent();
	GetPlayer2->HammerCountBarParent();
}

void AVersusController::SetAllInvisibleStageIcon()
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

void AVersusController::BattleFinish()
{
}

void AVersusController::CheckPlayerFall()
{
	if (!GetPlayer1)return;
	if (GetPlayer1->GetIsDeth())
	{
		// いかだとプレイヤーをリスポーンさせて相手のスコアを加算
		GetPlayer1->PlayerRespawn(Player1RespawnLocation);
	}

	if (!GetPlayer2)return;
	if (GetPlayer2->GetIsDeth())
	{
		GetPlayer2->PlayerRespawn(Player2RespawnLocation);
	}
}

void AVersusController::Goal(int BattleNumber)
{
	if (IsBatlleFinish) return;

	CreateBattleResultUI();
	BattleResultUI->SetWinPlayer(BattleNumber == 1);

	GetPlayer1->SetNoTick();
	GetPlayer2->SetNoTick();

	UE_LOG(LogTemp, Error, TEXT("Goal %d P"),BattleNumber);
	IsBatlleFinish = true;
}