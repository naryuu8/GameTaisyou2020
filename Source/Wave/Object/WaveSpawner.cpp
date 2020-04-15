// Fill out your copyright notice in the Description page of Project Settings.


#include "WaveSpawner.h"
#include "../WaterSurface/WaterSurface.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AWaveSpawner::AWaveSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AWaveSpawner::BeginPlay()
{
	Super::BeginPlay();
	WaveCountTime = WaveTime * 60.0f;
	FrameCounter = 0;
}

// Called every frame
void AWaveSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FrameCounter++;
	if (WaveCountTime == FrameCounter)
	{
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWaterSurface::StaticClass(), FoundActors);

		for (auto Actor : FoundActors)
		{
			AWaterSurface* water = Cast<AWaterSurface>(Actor);
			if (water)
			{
				water->AddPower(GetActorLocation());
			}
		}
		FrameCounter = 0;
	}
}

