// Fill out your copyright notice in the Description page of Project Settings.


#include "LandPoint.h"

// Sets default values
ALandPoint::ALandPoint()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ALandPoint::BeginPlay()
{
	Super::BeginPlay();
	IsUse = true;
}

// Called every frame
void ALandPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
//	DebugDraw();
}

bool ALandPoint::ShouldTickIfViewportsOnly() const
{
	return true;
}

