// Fill out your copyright notice in the Description page of Project Settings.


#include "Raft.h"
#include "WaterSurface.h"

void ARaft::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector CurrentPos = GetActorLocation();
	MoveVec = CurrentPos - PrevPos;
	PrevPos = CurrentPos;
}

FVector ARaft::AdjustMoveOnRaft(FVector actorPos, FVector moveVec)
{
	FVector movedPos = actorPos + moveVec;

	float xp = movedPos.X;
	float yp = movedPos.Y;
	float xs = GetActorLocation().X;
	float ys = GetActorLocation().Y;

	if (xp > xs + XLength * 0.5f) return actorPos - moveVec * 0.1f;
	if (xp < xs - XLength * 0.5f) return actorPos - moveVec * 0.1f;
	if (yp > ys + YLength * 0.5f) return actorPos - moveVec * 0.1f;
	if (yp < ys - YLength * 0.5f) return actorPos - moveVec * 0.1f;
	
	return movedPos;
}

bool ARaft::IsRide(FVector worldPos)
{
	return FVector::PointsAreNear(worldPos, GetActorLocation(), RideDirection);
}

bool ARaft::IsOnRaft(FVector worldPos)
{
	float xp = worldPos.X;
	float yp = worldPos.Y;
	float xs = GetActorLocation().X;
	float ys = GetActorLocation().Y;

	if (xp > xs + XLength * 0.5f) return false;
	if (xp < xs - XLength * 0.5f) return false;
	if (yp > ys + YLength * 0.5f) return false;
	if (yp < ys - YLength * 0.5f) return false;

	return true;
}

FVector ARaft::GetGetOffPos()
{
	return WaterSurface->GetGetOffPos(GetActorLocation(), RideDirection);
}