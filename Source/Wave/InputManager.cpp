// Fill out your copyright notice in the Description page of Project Settings.

#include "InputManager.h"
#include "Components/InputComponent.h"
#include "Kismet/GameplayStatics.h"
//入力クラスへのインスタンス
static AInputManager* InputManagerInstance = nullptr;

AInputManager::AInputManager()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickGroup = TG_LastDemotable;	// 更新グループを一番最後に登録する
	SetTickableWhenPaused(true);//ポーズ中でもTickが来るようにする
}

AInputManager::~AInputManager()
{
	InputManagerInstance = nullptr;
}

void AInputManager::BeginPlay()
{
	Super::BeginPlay();
	InputManagerInstance = this;
}

const AInputManager * AInputManager::GetInstance()
{
	//check(InputManagerInstance);
	return InputManagerInstance;
}

void AInputManager::Tick(float DeltaTime)
{
	State.Pause.Refresh();
	State.Up.Refresh();
	State.Down.Refresh();
	State.Select.Refresh();
//ポーズ中ならポーズボタン入力情報のみ更新
	if (UGameplayStatics::IsGamePaused(GetWorld()))return;
	State.Right.Refresh();
	State.Left.Refresh();
	State.Attack.Refresh();
	State.RightStickButton.Refresh();

}

void AInputManager::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AInputManager::InputLS_Vertical);
	PlayerInputComponent->BindAxis("MoveRight", this, &AInputManager::InputLS_Horizontal);

	PlayerInputComponent->BindAxis("LookUpRate", this, &AInputManager::InputRS_Vertical);
	PlayerInputComponent->BindAxis("TurnRate", this, &AInputManager::InputRS_Horizontal);

	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &AInputManager::InputAttackPress);
	PlayerInputComponent->BindAction("Attack", IE_Released, this, &AInputManager::InputAttackRelease);

	PlayerInputComponent->BindAction("ChangeCameraType", IE_Pressed, this, &AInputManager::InputRightStickButtonPress);
	PlayerInputComponent->BindAction("ChangeCameraType", IE_Released, this, &AInputManager::InputRightStickButtonRelease);

	//ポーズインプット
	//ポーズ中でもインプットが反応するようにbExecuteWhenPausedをtrueにする
	PlayerInputComponent->BindAction("Pause", IE_Pressed, this, &AInputManager::InputPausePress).bExecuteWhenPaused = true;
	PlayerInputComponent->BindAction("Pause", IE_Released, this, &AInputManager::InputPauseRelease).bExecuteWhenPaused = true;
	PlayerInputComponent->BindAction("Up", IE_Pressed, this, &AInputManager::InputUpPress).bExecuteWhenPaused = true;
	PlayerInputComponent->BindAction("Up", IE_Released, this, &AInputManager::InputUpRelease).bExecuteWhenPaused = true;
	PlayerInputComponent->BindAction("Down", IE_Pressed, this, &AInputManager::InputDownPress).bExecuteWhenPaused = true;
	PlayerInputComponent->BindAction("Down", IE_Released, this, &AInputManager::InputDownRelease).bExecuteWhenPaused = true;
	PlayerInputComponent->BindAction("Right", IE_Pressed, this, &AInputManager::InputRightPress).bExecuteWhenPaused = true;
	PlayerInputComponent->BindAction("Right", IE_Released, this, &AInputManager::InputRightRelease).bExecuteWhenPaused = true;
	PlayerInputComponent->BindAction("Left", IE_Pressed, this, &AInputManager::InputLeftPress).bExecuteWhenPaused = true;
	PlayerInputComponent->BindAction("Left", IE_Released, this, &AInputManager::InputLeftRelease).bExecuteWhenPaused = true;
	//決定ボタンインプット
	PlayerInputComponent->BindAction("Select", IE_Pressed, this, &AInputManager::InputSelectPress).bExecuteWhenPaused = true;
	PlayerInputComponent->BindAction("Select", IE_Released, this, &AInputManager::InputSelectRelease).bExecuteWhenPaused = true;
}

