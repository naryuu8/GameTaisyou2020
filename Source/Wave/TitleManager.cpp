// Fill out your copyright notice in the Description page of Project Settings.


#include "TitleManager.h"
#include "Camera/TitleCamera.h"
#include "Kismet/GameplayStatics.h"
#include "InputManager.h"
#include "Player/TitlePlayer.h"
#include "Blueprint/UserWidget.h"
#include "UI/TitleUI.h"
#include "GlobalGameInstance.h"
#include "UI/FadeUI.h"
#include "SoundManager.h"
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
	UGlobalGameInstance* instance = UGlobalGameInstance::GetInstance();
	if (instance)
	{//リザルト画面やポーズ画面からステージ選択に戻ったらステージセレクト画面からにする
		if (instance->GetIsStageSelectMode())
		{
			State = ETitleState::TitleMove;
			MoveFrameTime = StageSelectTime * 60.0f;
			SetCameraMove(TitleMoveCamera, 0.0f);
			IsNoInput = true;
			if (FadeUIClass != nullptr)
			{
				UFadeUI* fade = CreateWidget<UFadeUI>(GetWorld(), FadeUIClass);
				if (fade != nullptr)
				{
					fade->AddToViewport();
					fade->SetFade(FLinearColor(0.0f, 0.0f, 0.0f, 1.0f),false, 1.0f);
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
			instance->SetIsStageSelectMode(false);
			SetCameraMove(StageSelectCamera, 0.0f);
			StageSelectCamera->SetCenterTitleMemo();
			TitlePlayer->TargetRotation();
		}
		else
		{
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
		}
	}	
	MoveFrameCount = 0;
	MoveFrameTime = TitleMoveTime * 60.0f;

}

// Called every frame
void ATitleManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TitleInput();
	CameraMoveCheck();
	if (State == ETitleState::TitleMove)
	{
		TitlePlayer->TargetMove();
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
			SetCameraMove(TitleMoveCamera, TitleMoveTime);
			IsNoInput = true;
			TitlePlayer->SetIsSelect(true);
			TitleUI->RemoveFromParent();
		}
		if (State == ETitleState::StageSelect)
		{
			if (!StageSelectCamera->GetIsSelectMap())
			{
				StageSelectCamera->SelectInput();
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
	if (MoveFrameTime == MoveFrameCount)
	{
		MoveFrameCount = 0;
		if (State == ETitleState::Title)
		{
			MoveFrameTime = StageSelectTime * 60.0f;
			State = ETitleState::TitleMove;
			SetCameraMove(StageSelectCamera, StageSelectTime);
			TitlePlayer->TargetRotation();
		}
		else if (State == ETitleState::TitleMove)
		{
			State = ETitleState::StageSelect;
			IsNoInput = false;
		}
	}
	else
	{
		MoveFrameCount++;
	}
}

// Called to bind functionality to input
void ATitleManager::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

