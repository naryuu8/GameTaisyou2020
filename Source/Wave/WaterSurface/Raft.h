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
	
public:
	virtual void Tick(float DeltaTime) override;

	FVector AdjustMoveOnRaft(FVector actorPos, FVector moveVec);

	bool IsRide(FVector worldPos);

	bool IsOnRaft(FVector worldPos);

	float GetRiderZ() { return RiderZ; }

	FVector GetGetOffPos();

	FVector GetMoveVec() { return MoveVec; }
private:
	UPROPERTY(EditAnywhere)
		float RideDirection = 100.0f;

	UPROPERTY(EditAnywhere)
		float XLength = 50.0f;

	UPROPERTY(EditAnywhere)
		float YLength = 50.0f;

	UPROPERTY(EditAnywhere)
		float RiderZ = 50.0f;

	FVector MoveVec;
	FVector PrevPos;
};
