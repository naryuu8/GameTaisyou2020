// Fill out your copyright notice in the Description page of Project Settings.


#include "GameCameraFocusPoint.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

// Sets default values
AGameCameraFocusPoint::AGameCameraFocusPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	USceneComponent * Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	Scene->SetupAttachment(RootComponent);
}

void AGameCameraFocusPoint::SpawnFocusPoint(const AActor * Object, const FVector & Pos, float Time)
{
	AGameCameraFocusPoint * Actor = Cast<AGameCameraFocusPoint>(
		Object->GetWorld()->SpawnActor<AGameCameraFocusPoint>(Pos, FRotator::ZeroRotator));

	FTimerManager& timerManager = Object->GetWorld()->GetTimerManager();
	FTimerHandle handle;
	timerManager.SetTimer(handle, Actor, &AGameCameraFocusPoint::MyDestroy, Time);
}

void AGameCameraFocusPoint::BeginPlay()
{
	Super::BeginPlay();
	
}

void AGameCameraFocusPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGameCameraFocusPoint::MyDestroy()
{
	Destroy();
}
