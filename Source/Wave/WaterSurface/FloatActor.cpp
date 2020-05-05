// Fill out your copyright notice in the Description page of Project Settings.


#include "FloatActor.h"
#include "Kismet/GameplayStatics.h"
#include "WaterSurface.h"

// Sets default values
AFloatActor::AFloatActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AFloatActor::BeginPlay()
{
	Super::BeginPlay();
	
	WaterSurface = Cast<AWaterSurface>((UGameplayStatics::GetActorOfClass(GetWorld(), AWaterSurface::StaticClass())));
	
}

// Called every frame
void AFloatActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector MoveVec = WaterSurface->GetWavePower(GetActorLocation()) * WaterSurface->GetWaveSpeed();

	SetActorLocation(WaterSurface->AdjustMoveInWater(GetActorLocation(), MoveVec, 100.0f));
}