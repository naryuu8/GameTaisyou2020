// Fill out your copyright notice in the Description page of Project Settings.

#include "InputManager.h"
#include "Components/InputComponent.h"

//入力クラスへのインスタンス
static AInputManager* InputManagerInstance = nullptr;

// Sets default values
AInputManager::AInputManager()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AInputManager::BeginPlay()
{
	Super::BeginPlay();
	InputManagerInstance = this;
}

AInputManager * AInputManager::GetInstance()
{
	check(InputManagerInstance);
	return InputManagerInstance;
}

void AInputManager::Tick(float DeltaTime)
{
	State.Attack.Refresh();
}

// Called to bind functionality to input
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

