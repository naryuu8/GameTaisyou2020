// Fill out your copyright notice in the Description page of Project Settings.


#include "SquareLand.h"
#include "Runtime/Engine/Classes/Kismet//GameplayStatics.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"

void ASquareLand::DebugDraw()
{
	float XOffset = XLength * 0.5f;
	float YOffset = YLength * 0.5f;
	// デバッグ用の四角を描画(高さや太さは決め打ち）
	UKismetSystemLibrary::DrawDebugLine(this, GetActorLocation() + FVector( XOffset,  YOffset, 0.0f), GetActorLocation() + FVector(-XOffset,  YOffset, 0.0f), LAND_TYPE_COLORS[GetLandTypeNumber()], 0.0f, 3.0f);
	UKismetSystemLibrary::DrawDebugLine(this, GetActorLocation() + FVector( XOffset, -YOffset, 0.0f), GetActorLocation() + FVector(-XOffset, -YOffset, 0.0f), LAND_TYPE_COLORS[GetLandTypeNumber()], 0.0f, 3.0f);
	UKismetSystemLibrary::DrawDebugLine(this, GetActorLocation() + FVector( XOffset,  YOffset, 0.0f), GetActorLocation() + FVector( XOffset, -YOffset, 0.0f), LAND_TYPE_COLORS[GetLandTypeNumber()], 0.0f, 3.0f);
	UKismetSystemLibrary::DrawDebugLine(this, GetActorLocation() + FVector(-XOffset,  YOffset, 0.0f), GetActorLocation() + FVector(-XOffset, -YOffset, 0.0f), LAND_TYPE_COLORS[GetLandTypeNumber()], 0.0f, 3.0f);
}