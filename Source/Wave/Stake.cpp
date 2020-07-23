// Fill out your copyright notice in the Description page of Project Settings.


#include "Stake.h"

// Sets default values
AStake::AStake()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AStake::BeginPlay()
{
	Super::BeginPlay();

	IsOn = false;
}

void AStake::Push(const FVector Point)
{
	if (IsOn) return;

	float distance = FVector::Distance(Point, GetActorLocation());
	if (distance > 100) return;

	IsOn = true;

	FVector Location = GetActorLocation();
	Location.Z -= 50;
	SetActorLocation(Location);
}