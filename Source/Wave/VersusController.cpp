// Fill out your copyright notice in the Description page of Project Settings.


#include "VersusController.h"
#include "SectionController.h"
#include "Kismet/GameplayStatics.h"
#include "WaterSurface/WaterSurface.h"
#include "UI/BattleResultUI.h"
#include "InputManager.h"
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
}

void AVersusController::CreateSections()
{
	for (int i = 0; i < SectionCount; i++)
	{
		int index = FMath::RandRange(0, Sections.Num() - 1);
		AddSection(index, 1, i);
	}
}

void AVersusController::AddSection(int index, int PlayerNumber, int Cnt)
{
	FVector Location = FVector::ZeroVector;
	Location.X = SectionSize * Cnt;
	ASectionController* section = GetWorld()->SpawnActor<ASectionController>(Sections[index]);
	section->SetActorLocation(Location);
	(PlayerNumber == 1 ? Player1Sections : Player2Sections).Add(section);
}

// Called every frame
void AVersusController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AVersusController::CreateBattleResultUI()
{
	if (BattleResultUI)return;
	if (BattleResultUIClass != nullptr)
	{
		BattleResultUI = CreateWidget<UBattleResultUI>(GetWorld(), BattleResultUIClass);
		if (BattleResultUI != nullptr)
		{
			BattleResultUI->SetWinPlayer(true);
			BattleResultUI->AddToViewport();
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
