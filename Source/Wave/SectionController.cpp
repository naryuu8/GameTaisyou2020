// Fill out your copyright notice in the Description page of Project Settings.

#include "SectionController.h"
#include "Object/GoalComponent.h"
#include "Scaffold.h"
#include "TimerManager.h"
#include "Stake.h"

// Sets default values
ASectionController::ASectionController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASectionController::BeginPlay()
{
	Super::BeginPlay();

	SetfromBP();

	State = ESectionState::Challenge;
	IsClear = false;

	for (AActor* Actor : GoalActors)
	{
		if (!Actor) continue;
		UActorComponent* Component = Actor->GetComponentByClass(UGoalComponent::StaticClass());
		UGoalComponent* GoalComponent = Cast<UGoalComponent>(Component);
		if (GoalComponent)
		{
			Goals.Add(GoalComponent);
		}
	}
}

// Called every frame
void ASectionController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (State == ESectionState::None) return;
	if (State == ESectionState::Clear) return;

	if (IsClear) return;
	if (CheckClear())
	{
		OnClear();
		State = ESectionState::Clear;
	}
}

bool ASectionController::CheckClear()
{
	for (UGoalComponent* Goal : Goals)
	{
		if (!Goal->GetIsGoal()) return false;
	}

	for (AStake* Stake: Stakes)
	{
		if (!Stake->GetIsOn()) return false;
	}

	return true;
}

void ASectionController::OnClear()
{
	FTimerManager& timerManager = GetWorld()->GetTimerManager();
	float time = 0.1f;
	for (AScaffold* Scaffold : Scaffolds)
	{
		FTimerHandle handle;
		timerManager.SetTimer(handle, Scaffold, &AScaffold::Clear, time);
		time += 1.0f;
	}
}
