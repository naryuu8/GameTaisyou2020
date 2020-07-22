// Fill out your copyright notice in the Description page of Project Settings.


#include "VersusController.h"
#include "SectionController.h"
#include "Kismet/GameplayStatics.h"
#include "WaterSurface/WaterSurface.h"

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

