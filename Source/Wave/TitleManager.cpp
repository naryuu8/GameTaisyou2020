// Fill out your copyright notice in the Description page of Project Settings.


#include "TitleManager.h"
#include "Camera/TitleCamera.h"
#include "Camera/Title_BattleCamera.h"
#include "Kismet/GameplayStatics.h"
#include "InputManager.h"
#include "Player/TitlePlayer.h"
#include "Blueprint/UserWidget.h"
#include "UI/TitleUI.h"
#include "UI/TitleTipsUI.h"
#include "GlobalGameInstance.h"
#include "UI/FadeUI.h"
#include "SoundManager.h"
#include "TimerManager.h"
// Sets default values
ATitleManager::ATitleManager()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATitleManager::BeginPlay()
{
	Super::BeginPlay();
	// 画面分割無効
	GetWorld()->GetGameViewport()->SetForceDisableSplitscreen(true);
	UGlobalGameInstance* instance = UGlobalGameInstance::GetInstance();
	if (instance)
	{//リザルト画面やポーズ画面からステージ選択に戻ったらステージセレクト画面からにする
		if (instance->GetIsStageSelectMode())
		{
			State = ETitleState::TitleMove;
			SetCameraMove(TitleMoveCamera, 0.0f);
			IsNoInput = true;
			instance->SetIsStageSelectMode(false);
			SetCameraMove(StageSelectCamera, 0.0f);
			StageSelectCamera->SetCenterTitleMemo();
			TitlePlayer->TargetRotation(IsBattleMode);
			CreateTitleTipsUI();
			CreateFadeUI(true);
		}
		else
		{
			if (!AudioComponent) AudioComponent = ASoundManager::CreateAudioComponent(SOUND_TYPE::TITLE_BGM);
			if (AudioComponent)
			{
				if (!(AudioComponent->IsPlaying())) AudioComponent->Play();
			}
			IsNoInput = false;
			State = ETitleState::Title;
			APlayerController *playerControtller = UGameplayStatics::GetPlayerController(this, 0);
			if (playerControtller)
			{
				playerControtller->SetViewTargetWithBlend(TitleCamera, 0.0f, EViewTargetBlendFunction::VTBlend_Linear, 0.0f, false);
			}
			TitleUI = CreateWidget<UTitleUI>(GetWorld(), TitleUIClass);
			if (TitleUI)
			{
				TitleUI->AddToViewport();
			}
			CreateTitleTipsUI();
			if (TitleTipsUI)
			{
				TitleTipsUI->SetNoDraw();
			}
			StageSelectCamera->SetMyStageNumber(0);
			CreateFadeUI(false);
		}
	}	

}

// Called every frame
void ATitleManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TitleInput();
	CameraMoveCheck();
	if (State == ETitleState::TitleMove)
	{
		TitlePlayer->TargetMove(IsBattleMode);
	}
}

void ATitleManager::TitleInput()
{
	if (IsNoInput)return;
	const AInputManager * inputManager = AInputManager::GetInstance();
	if (!inputManager)return;
	const InputState * input = inputManager->GetState();
	if (input->Select.IsPress)
	{
		if (State == ETitleState::Title)
		{
			StageSelectCamera->SetMyStageNumber(0);
			CheckTitleSelectNumber();
		}
		if (State == ETitleState::StageSelect)
		{
			if (IsBattleMode)
			{
				if (!TitleBattleCamera->GetIsSelectMap())
				{
					ASoundManager::SafePlaySound(SOUND_TYPE::MENU_DECISION);
					TitleBattleCamera->SelectInput();
				}
			}
			else
			{
				if (!StageSelectCamera->GetIsSelectMap())
				{
					ASoundManager::SafePlaySound(SOUND_TYPE::MENU_DECISION);
					StageSelectCamera->SelectInput();
				}
			}
		}
	}
	if (State == ETitleState::Title)
	{
		if (input->Up.IsPress)
		{
			if (TitleUI)
			{
				TitleUI->BackSelectState();
			}
		}
		if (input->Down.IsPress)
		{
			if (TitleUI)
			{
				TitleUI->NextSelectState();
			}
		}
	}
	if (State == ETitleState::StageSelect)
	{
		if (input->Right.IsPress)
		{
			StageSelectCamera->RightInput();
			ASoundManager::SafePlaySound(SOUND_TYPE::STAGE_SELECT);
			
		}
		if (input->Left.IsPress)
		{
			StageSelectCamera->LeftInput();
			ASoundManager::SafePlaySound(SOUND_TYPE::STAGE_SELECT);
		}
		if (input->Back.IsPress)
		{
			if (IsBattleMode)
			{
				TitleBattleCamera->SetBack();
			}
			else
			{
				StageSelectCamera->SetBack();
			}	
			IsNoInput = true;
			if (FadeUI)
			{
				FadeUI->SetFadeLevel(FLinearColor(0.0f, 0.0f, 0.0f, 1.0f), 1.0f, *UGameplayStatics::GetCurrentLevelName(GetWorld()), false);
			}
			if (TitleTipsUI)
			{
				TitleTipsUI->RemoveFromParent();
			}
			ASoundManager::SafePlaySound(SOUND_TYPE::STAGE_SELECT);
		}
	}
}

void ATitleManager::SetCameraMove(AActor * camera, const float camera_speed)
{
	APlayerController *playerControtller = UGameplayStatics::GetPlayerController(this, 0);
	if (playerControtller)
	{
		playerControtller->SetViewTargetWithBlend(camera, camera_speed, EViewTargetBlendFunction::VTBlend_Linear, 0.0f, false);
	}
}

void ATitleManager::CameraMoveCheck()
{
	if (!IsNoInput)return;	
	if (State == ETitleState::Title)
	{
		FTimerManager& timerManager = GetWorld()->GetTimerManager();
		FTimerHandle handle;
		timerManager.SetTimer(handle, this, &ATitleManager::SetTitleMoveState, TitleMoveTime);
	
	}
	else if (State == ETitleState::TitleMove)
	{
		FTimerManager& timerManager = GetWorld()->GetTimerManager();
		FTimerHandle handle;
		timerManager.SetTimer(handle, this, &ATitleManager::StageSelectState, StageSelectTime);

		if (!SelectAudioComponent) SelectAudioComponent = ASoundManager::CreateAudioComponent(SOUND_TYPE::SELECT_BGM);

		if (SelectAudioComponent)
		{
			SelectAudioComponent->Play();
		}
	}
}

// Called to bind functionality to input
void ATitleManager::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ATitleManager::SetTitleMoveState()
{
	State = ETitleState::TitleMove;

	if (IsBattleMode)
	{
		SetCameraMove(TitleBattleCamera, StageSelectTime);
	}
	else
	{
		SetCameraMove(StageSelectCamera, StageSelectTime);
	}
	TitlePlayer->TargetRotation(IsBattleMode);
}

void ATitleManager::StageSelectState()
{
	State = ETitleState::StageSelect;

	if (AudioComponent)
	{
		AudioComponent->Stop();
	}
	TitleTipsUI->SetDraw();
	IsNoInput = false;
}

void ATitleManager::CheckTitleSelectNumber()
{
	if (TitleUI)
	{	
		switch (TitleUI->GetSelectNumber())
		{
			case static_cast<int>(TitleSelectState::GAME_START) :
				SetCameraMove(TitleMoveCamera, TitleMoveTime);
				IsNoInput = true;
				TitlePlayer->SetIsSelect(true);
				TitleUI->RemoveFromParent();
				ASoundManager::SafePlaySound(SOUND_TYPE::MENU_DECISION);
				break;
				case static_cast<int>(TitleSelectState::BATTLE_MODE) :
					IsBattleMode = true;
					SetCameraMove(TitleBattleMoveCamera, TitleMoveTime);
					IsNoInput = true;
					TitlePlayer->SetIsSelect(true);
					TitleUI->RemoveFromParent();
					ASoundManager::SafePlaySound(SOUND_TYPE::MENU_DECISION);
					break;
					case static_cast<int>(TitleSelectState::DATA_DELETE) :
						IsNoInput = true;
						TitleUI->DeleteSaveData();
						if (FadeUI)
						{
							FadeUI->SetFadeLevel(FLinearColor(0.0f, 0.0f, 0.0f, 1.0f), 1.0f, *UGameplayStatics::GetCurrentLevelName(GetWorld()), false);
						}
						ASoundManager::SafePlaySound(SOUND_TYPE::MENU_DECISION);
						break;
						case static_cast<int>(TitleSelectState::GAME_END) :
							TitleUI->EndGame();
							ASoundManager::SafePlaySound(SOUND_TYPE::MENU_DECISION);
							IsNoInput = true;
							if (FadeUI)
							{
								FadeUI->SetFade(FLinearColor(0.0f, 0.0f, 0.0f, 1.0f), true, 1.0f, false);
							}
							break;
		}
	}
}

void ATitleManager::CreateFadeUI(const bool select)
{
	if (FadeUI)return;
	if (FadeUIClass != nullptr)
	{
		FadeUI = CreateWidget<UFadeUI>(GetWorld(), FadeUIClass);
		if (FadeUI != nullptr)
		{
			FadeUI->AddToViewport();
			if (select)
			{
				FadeUI->SetFade(FLinearColor(0.0f, 0.0f, 0.0f, 1.0f), false, 1.5f, true);
			}
			else
			{
				FadeUI->SetFade(FLinearColor(0.0f, 0.0f, 0.0f, 1.0f), false, 1.0f, false);
			}
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

void ATitleManager::CreateTitleTipsUI()
{
	if (TitleTipsUIClass != nullptr)
	{
		TitleTipsUI = CreateWidget<UTitleTipsUI>(GetWorld(), TitleTipsUIClass);
		if (TitleTipsUI != nullptr)
		{
			TitleTipsUI->AddToViewport();
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("TitleTipsUIClass : %s"), L"Widget cannot create");
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("TitleTipsUIClass : %s"), L"UIClass is nullptr");
	}
}