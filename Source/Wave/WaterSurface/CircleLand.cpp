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
	FVector2D CirclePos = FVector2D(GetActorLocation().X, GetActorLocation().Y);
	XLen = XLen * 0.5f;
	YLen = YLen * 0.5f;

	float XDistance = (CirclePos.X - MovedPos.X);
	float YDistance = (CirclePos.Y - MovedPos.Y);
	float JudgXDistance = Radius + XLen;
	float JudgYDistance = Radius + YLen;

	float X_Deff = JudgXDistance - fabs(XDistance);
	float Y_Deff = JudgYDistance - fabs(YDistance);

	if (X_Deff < 0 || Y_Deff < 0) return MovedPos;

	// 衝突している

	FRay2DCastInfo Info;
	FVector2D VecA, VecB, VecC, VecD;
	// レイの交点を調べて距離を取得する関数
	VecA = CirclePos + FVector2D(XLen, YLen);
	VecB = CirclePos + FVector2D(-XLen, YLen);
	VecC = CirclePos + FVector2D(-XLen, -YLen);
	VecD = CirclePos + FVector2D(XLen, -YLen);

	FRay2D PosToMovedPos = FRay2D(OldPos, MovedPos - OldPos);
	MyFunc::Check_CircleRay2D_VS_Ray2D(Info, PosToMovedPos, Radius, FRay2D(VecA, VecB - VecA), FVector2D(0.0f, 1.0f));
	MyFunc::Check_CircleRay2D_VS_Ray2D(Info, PosToMovedPos, Radius, FRay2D(VecB, VecC - VecB), FVector2D(-1.0f, 0.0f));
	MyFunc::Check_CircleRay2D_VS_Ray2D(Info, PosToMovedPos, Radius, FRay2D(VecC, VecD - VecC), FVector2D(0.0f, -1.0f));
	MyFunc::Check_CircleRay2D_VS_Ray2D(Info, PosToMovedPos, Radius, FRay2D(VecD, VecA - VecD), FVector2D(1.0f, 0.0f));

	// 辺に衝突していた時
	if (Info.IsHit)
	{
		// 衝突点に位置を修正
		MovedPos = FVector(Info.NearPos, MovedPos.Z);
		// 反射ベクトルを算出
		FVector2D Ref = MyFunc::GetReflectVector2D(PosToMovedPos.Direction, Info.NearNormal);
		MoveVec = FVector(Ref, 0.0f);
	}
	// 既に埋まっている状態
	else
	{
		// X軸の押し出し（ないとは思うけど一応埋まり防止用）
		MovedPos.X += (XDistance > 0) ? -X_Deff : X_Deff;
	}

	return MovedPos;
}
