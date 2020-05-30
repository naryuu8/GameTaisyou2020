// Fill out your copyright notice in the Description page of Project Settings.


#include "CircleLand.h"
#include "Runtime/Engine/Classes/Kismet//GameplayStatics.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"

void ACircleLand::DebugDraw()
{
	// デバッグ用のサークルを描画(高さや太さは決め打ち）
	UKismetSystemLibrary::DrawDebugCylinder(this, GetActorLocation(), GetActorLocation() - FVector(0,0,10), Radius, 64, LAND_TYPE_COLORS[GetLandTypeNumber()], 0.0f, 3.0f);
}

bool ACircleLand::OnGround(const FVector & Pos)
{
	FVector Dist = GetActorLocation() - Pos;
	return Dist.SizeSquared() < Radius * Radius;
}

FVector ACircleLand::AdjustMoveInLand(const FVector & Pos, float CircleRadius)
{
	FVector Result = Pos;
	// 円の外に出ないようにする処理で行けるはず
	FVector CircleCenter = GetActorLocation();
	FVector MovedPosDir = Pos - CircleCenter;
	float Deff = MovedPosDir.Size() + CircleRadius - GetRadius();
	// 円の外にはみ出た場合のみ位置を移動させる
	if (Deff > 0)
	{
		MovedPosDir.Normalize();
		Result -= MovedPosDir * Deff;
	}
	return Result;
}
