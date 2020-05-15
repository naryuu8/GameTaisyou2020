// Fill out your copyright notice in the Description page of Project Settings.


#include "Rock.h"
#include "Kismet/GameplayStatics.h"
#include "WaterSurface.h"

// Sets default values
ARock::ARock()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARock::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWaterSurface::StaticClass(), FoundActors);
	for (auto Actor : FoundActors)
	{
		AWaterSurface* water = Cast<AWaterSurface>(Actor);
		if (water)
		{
			Water = water;
		}
	}

	IsWave = true;
}

// Called every frame
void ARock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float Power = Water->GetWavePower(GetActorLocation()).Size();

	if (IsWave && Power > 1)
	{
		Water->AddPower(GetActorLocation(), Power * Magnification);
		IsWave = false;
		UE_LOG(LogTemp, Log, TEXT("Rock = %f"), Power);
	}
	else if (Power < 0.1f)
	{
		IsWave = true;
	}
}

