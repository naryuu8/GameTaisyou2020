// Fill out your copyright notice in the Description page of Project Settings.


#include "CircleLand.h"
#include "Runtime/Engine/Classes/Kismet//GameplayStatics.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "../MyFunc.h"

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

bool ACircleLand::OnGround(const FVector & Pos, float CircleRadius)
{
	FVector Dist = GetActorLocation() - Pos;
	Dist -= Dist.GetSafeNormal() * CircleRadius;
	return Dist.SizeSquared() < Radius * Radius;
}

bool ACircleLand::InGround(const FVector & Pos, float CircleRadius)
{
	FVector Dist = GetActorLocation() - Pos;
	Dist += Dist.GetSafeNormal() * CircleRadius;
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

FVector ACircleLand::AdjustMoveOutWater(const FVector & OldPos, FVector MovedPos, FVector & MoveVec, float CircleRadius)
{
	float distance = FVector::Distance(MovedPos, GetActorLocation());
	float judgDistance = CircleRadius + GetRadius();
	float landingDistance = judgDistance - distance;
	if (landingDistance <= 0) return MovedPos;

	FVector outDirection = MovedPos - GetActorLocation();
	outDirection.Z = 0;
	outDirection.Normalize();

	MovedPos = MovedPos + outDirection * landingDistance;

	FVector2D MoveDirection = FVector2D(MovedPos - OldPos);
	FVector2D Ref = (MoveDirection - 2.0f * FVector2D::DotProduct(MoveDirection, FVector2D(outDirection)) * FVector2D(outDirection));
	MoveVec = FVector(Ref, 0.0f);

	return MovedPos;
}

FVector ACircleLand::AdjustMoveOutWater(const FVector & OldPos, FVector MovedPos, FVector & MoveVec, float XLen, float YLen)
{
	FVector2D CirclePos = FVector2D(GetActorLocation());
	XLen = XLen * 0.5f;
	YLen = YLen * 0.5f;

	FRay2DCastInfo Info;
	FVector2D VecA, VecB, VecC, VecD;
	VecA = (FVector2D)MovedPos + FVector2D(XLen, YLen);
	VecB = (FVector2D)MovedPos + FVector2D(-XLen, YLen);
	VecC = (FVector2D)MovedPos + FVector2D(-XLen, -YLen);
	VecD = (FVector2D)MovedPos + FVector2D(XLen, -YLen);
	MyFunc::Check_Ray2D_VS_Circle(Info, FRay2D(VecA, VecB - VecA), CirclePos, Radius);
	MyFunc::Check_Ray2D_VS_Circle(Info, FRay2D(VecB, VecC - VecB), CirclePos, Radius);
	MyFunc::Check_Ray2D_VS_Circle(Info, FRay2D(VecC, VecD - VecC), CirclePos, Radius);
	MyFunc::Check_Ray2D_VS_Circle(Info, FRay2D(VecD, VecA - VecD), CirclePos, Radius);
	if (!Info.IsHit)
	{
		// 完全に埋まっている場合
		if (VecA.X > CirclePos.X && VecC.X < CirclePos.X &&
			VecA.Y > CirclePos.Y && VecC.Y < CirclePos.Y)
			return OldPos;

		return MovedPos;
	}
	// 衝突している
	float PushValue = (Radius - Info.HitDist);
	MovedPos += FVector(Info.NearNormal * PushValue, 0.0f);
	MoveVec = FVector(MyFunc::GetReflectVector2D((FVector2D)MoveVec, Info.NearNormal), 0.0f);

	return MovedPos;
}
