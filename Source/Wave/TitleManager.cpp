// Fill out your copyright notice in the Description page of Project Settings.


#include "TitleManager.h"
#include "Camera/TitleCamera.h"
#include "Kismet/GameplayStatics.h"
#include "InputManager.h"
#include "Player/TitlePlayer.h"
#include "Blueprint/UserWidget.h"
#include "UI/TitleUI.h"

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
	State = ETitleState::Title;
	IsNoInput = false;
	MoveFrameCount = 0;
	MoveFrameTime = TitleMoveTime * 60.0f;
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
		}
		if (input->Left.IsPress)
		{
			StageSelectCamera->LeftInput();
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

