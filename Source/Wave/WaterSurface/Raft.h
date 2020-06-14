// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FloatActor.h"
#include "Raft.generated.h"

/**
 * 
 */
UCLASS()
class WAVE_API ARaft : public AFloatActor
{
	GENERATED_BODY()
	
protected:
public:
	virtual void DebugDraw() override;
	virtual void Tick(float DeltaTime) override;

	FVector AdjustMoveOnRaft(const FVector & actorPos, const FVector & moveVec, float CircleRadius);
	bool IsOnRaft(FVector worldPos);
	bool IsInRaft(const FVector & worldPos, float CircleRadius);
	void StopRaftAudio();
private:
	class UAudioComponent* AudioComponent;
};
