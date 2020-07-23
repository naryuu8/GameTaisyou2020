// Fill out your copyright notice in the Description page of Project Settings.

#include "InputManager.h"
#include "Components/InputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Engine/World.h"

#define MAX_INPUT_INSTANCE (2)

//入力クラスへのインスタンス
static AInputManager* InputManagerInstances[MAX_INPUT_INSTANCE] = {};

AInputManager::AInputManager()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickGroup = TG_LastDemotable;	// 更新グループを一番最後に登録する
	SetTickableWhenPaused(true);//ポーズ中でもTickが来るようにする
}

AInputManager::~AInputManager()
{
	InputManagerInstances[AutoPossessPlayer - 1] = nullptr;
}

void AInputManager::BeginPlay()
{
	Super::BeginPlay();

	// 一つ目に生成されたインスタンスが残りのインスタンスを生成する
	// 二つ目以降は既に登録済みなのでリターン
	if (InputManagerInstances[0]) return;

	// 入力イベントを受け取る番号を指定
	this->AutoPossessPlayer = EAutoReceiveInput::Player0;
	this->AutoReceiveInput = this->AutoPossessPlayer;
	InputManagerInstances[0] = this;
	
	for (int i = 1; i < MAX_INPUT_INSTANCE; i++)
	{
		UGameplayStatics::CreatePlayer(GetWorld(), i);
	}

	TArray<AActor*> actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AInputManager::StaticClass(), actors);

	int num = 1;
	for (auto actor : actors)
	{
		if (actor == this) continue;

		InputManagerInstances[num] = Cast<AInputManager>(actor);
		num++;
	}

	//for (int i = 1; i < MAX_INPUT_INSTANCE; i++)
	//{
	//	AInputManager * Instance = GetWorld()->SpawnActor<AInputManager>();
	//	// 入力イベントを受け取る番号を指定
	//	Instance->AutoPossessPlayer = (EAutoReceiveInput::Type)(i + 1);
	//	Instance->AutoReceiveInput = Instance->AutoPossessPlayer;
	//	Instance->SetPlayerState(GetPlayerState());
	//	Instance->Controller = UGameplayStatics::CreatePlayer(GetWorld(), i);
	//	if (Instance->Controller)
	//	{
	//		Instance->SetupPlayerInputComponent(Instance->Controller->InputComponent);
	//		InputManagerInstances[i] = Instance;
	//	}
	//}
}

const AInputManager * AInputManager::GetInstance(EAutoReceiveInput::Type Index)
{
	if (MAX_INPUT_INSTANCE < Index)
		return nullptr;

	return InputManagerInstances[Index - 1];
}

void AInputManager::Tick(float DeltaTime)
{
	State.Pause.Refresh();
	State.Up.Refresh();
	State.Down.Refresh();
	State.Select.Refresh();
	State.Back.Refresh();
	State.Right.Refresh();
	State.Left.Refresh();
//ポーズ中ならポーズボタン入力情報のみ更新
	if (UGameplayStatics::IsGamePaused(GetWorld()))return;
	State.Attack.Refresh();
	State.AttackCancel.Refresh();
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

	PlayerInputComponent->BindAction("AttackCancel", IE_Pressed, this, &AInputManager::InputAttackCancelPress);
	PlayerInputComponent->BindAction("AttackCancel", IE_Released, this, &AInputManager::InputAttackCancelRelease);

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
	//戻るボタンインプット
	PlayerInputComponent->BindAction("Back", IE_Pressed, this, &AInputManager::InputBackPress).bExecuteWhenPaused = true;
	PlayerInputComponent->BindAction("Back", IE_Released, this, &AInputManager::InputBackRelease).bExecuteWhenPaused = true;
}

