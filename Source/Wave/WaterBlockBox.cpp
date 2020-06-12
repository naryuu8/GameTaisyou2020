// Fill out your copyright notice in the Description page of Project Settings.


#include "WaterBlockBox.h"
#include "Player/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"

void AWaterBlockBox::BeginPlay()
{
	Super::BeginPlay();
	GetPlayer = Cast<APlayerCharacter>(UGameplayStatics::GetActorOfClass(GetWorld(), APlayerCharacter::StaticClass()));
	SetNoCollision();
}

void AWaterBlockBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (GetPlayer->GetIsDeth())
	{
		SetONCollision();
	}
}