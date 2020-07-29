// Fill out your copyright notice in the Description page of Project Settings.


#include "Flag.h"
#include "../Player/PlayerCharacter.h"
#include "../VersusController.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AFlag::AFlag()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFlag::BeginPlay()
{
	Super::BeginPlay();
	IsGoal = false;
}

// Called every frame
void AFlag::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFlag::OnPlayerCheck(AActor * OtherActor)
{
	if (IsGoal) return;
	APlayerCharacter* OtherPlayer = Cast<APlayerCharacter>(OtherActor);

	if (!OtherPlayer) return;

	AVersusController* Versus = Cast<AVersusController>(UGameplayStatics::GetActorOfClass(GetWorld(), AVersusController::StaticClass()));
	if (Versus)
		Versus->Goal(BattleNumber);

	IsGoal = true;
}


