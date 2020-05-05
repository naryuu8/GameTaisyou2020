// Fill out your copyright notice in the Description page of Project Settings.

#include "InputManager.h"
#include "Components/InputComponent.h"

//入力クラスへのインスタンス
static AInputManager* InputManagerInstance = nullptr;

AInputManager::AInputManager()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickGroup = TG_LastDemotable;	// 更新グループを一番最後に登録する
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
	State.Attack.Refresh();
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
}

