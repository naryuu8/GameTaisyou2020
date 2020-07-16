// Fill out your copyright notice in the Description page of Project Settings.


#include "TitleBattleMemo.h"
#include "../UI/FadeUI.h"

// Sets default values
ATitleBattleMemo::ATitleBattleMemo()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATitleBattleMemo::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATitleBattleMemo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATitleBattleMemo::FadeStart()
{
	UFadeUI* fade = CreateWidget<UFadeUI>(GetWorld(), FadeUIClass);
	if (fade)
	{
		if (fade->GetFadeIsEnable())return;
		fade->AddToViewport();
		fade->SetFadeLevel(FColor::Black, 1.0f, FName(*StageLevelPath), true);
	}
}