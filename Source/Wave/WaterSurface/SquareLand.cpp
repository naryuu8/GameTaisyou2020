// Fill out your copyright notice in the Description page of Project Settings.


#include "SquareLand.h"
#include "Runtime/Engine/Classes/Kismet//GameplayStatics.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "../MyFunc.h"

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

bool ASquareLand::OnGround(const FVector & Pos)
{
	FVector2D SquarePos = FVector2D(GetActorLocation().X, GetActorLocation().Y);
	float XLen = GetXLength() * 0.5f;
	float YLen = GetYLength() * 0.5f;

	return  SquarePos.X - XLen < Pos.X &&
			SquarePos.X + XLen > Pos.X &&
			SquarePos.Y - YLen < Pos.Y &&
			SquarePos.Y + YLen > Pos.Y;
}

bool ASquareLand::OnGround(const FVector & Pos, float CircleRadius)
{
	FVector2D SquarePos = FVector2D(GetActorLocation().X, GetActorLocation().Y);
	float XLen = GetXLength() * 0.5f;
	float YLen = GetYLength() * 0.5f;

	return  SquarePos.X - XLen < Pos.X + CircleRadius &&
			SquarePos.X + XLen > Pos.X - CircleRadius &&
			SquarePos.Y - YLen < Pos.Y + CircleRadius &&
			SquarePos.Y + YLen > Pos.Y - CircleRadius;
}

bool ASquareLand::InGround(const FVector & Pos, float CircleRadius)
{
	FVector2D SquarePos = FVector2D(GetActorLocation().X, GetActorLocation().Y);
	float XLen = GetXLength() * 0.5f;
	float YLen = GetYLength() * 0.5f;

	return  SquarePos.X - XLen < Pos.X - CircleRadius &&
			SquarePos.X + XLen > Pos.X + CircleRadius &&
			SquarePos.Y - YLen < Pos.Y - CircleRadius &&
			SquarePos.Y + YLen > Pos.Y + CircleRadius;
}

FVector ASquareLand::AdjustMoveInLand(const FVector & Pos, float CircleRadius)
{
	FVector Result = Pos;
	FVector SquareCenter = GetActorLocation();
	float X_HalfLen = GetXLength() * 0.5f;
	float Y_HalfLen = GetYLength() * 0.5f;
	// X軸を確認
	float Deff = Result.X +CircleRadius - (SquareCenter.X + X_HalfLen);
	if (Deff > 0.0f) Result.X -= Deff;
	else
	{
		Deff = Result.X - CircleRadius - (SquareCenter.X - X_HalfLen);
		if (Deff < 0.0f) Result.X -= Deff;
	}
	// Y軸を確認
	Deff = Result.Y + CircleRadius - (SquareCenter.Y + Y_HalfLen);
	if (Deff > 0.0f) Result.Y -= Deff;
	else
	{
		Deff = Result.Y - CircleRadius - (SquareCenter.Y - Y_HalfLen);
		if (Deff < 0.0f) Result.Y -= Deff;
	}

	return Result;
}

FVector ASquareLand::AdjustMoveOutWater(const FVector & OldPos, FVector MovedPos, FVector & MoveVec, float CircleRadius, float Repulsion)
{
	FVector2D SquarePos = FVector2D(GetActorLocation());
	float XLen = GetXLength() * 0.5f;
	float YLen = GetYLength() * 0.5f;

	FRay2DCastInfo Info;
	FVector2D VecA, VecB, VecC, VecD;
	VecA = (FVector2D)SquarePos + FVector2D(XLen, YLen);
	VecB = (FVector2D)SquarePos + FVector2D(-XLen, YLen);
	VecC = (FVector2D)SquarePos + FVector2D(-XLen, -YLen);
	VecD = (FVector2D)SquarePos + FVector2D(XLen, -YLen);
	MyFunc::Check_Ray2D_VS_Circle(Info, FRay2D(VecA, VecB - VecA), (FVector2D)MovedPos, CircleRadius);
	MyFunc::Check_Ray2D_VS_Circle(Info, FRay2D(VecB, VecC - VecB), (FVector2D)MovedPos, CircleRadius);
	MyFunc::Check_Ray2D_VS_Circle(Info, FRay2D(VecC, VecD - VecC), (FVector2D)MovedPos, CircleRadius);
	MyFunc::Check_Ray2D_VS_Circle(Info, FRay2D(VecD, VecA - VecD), (FVector2D)MovedPos, CircleRadius);
	if (!Info.IsHit)
	{
		// 完全に埋まっている場合
		if (VecA.X > MovedPos.X && VecC.X < MovedPos.X &&
			VecA.Y > MovedPos.Y && VecC.Y < MovedPos.Y)
			return OldPos;

		return MovedPos;
	}

	// 衝突している
	float PushValue = (CircleRadius - Info.HitDist) * Repulsion;
	MovedPos += FVector(-Info.NearNormal * PushValue, 0.0f);
	MoveVec = FVector(MyFunc::GetReflectVector2D((FVector2D)MoveVec, -Info.NearNormal), 0.0f);
	MoveVec -= FVector(-Info.NearNormal, 0.0f) * FVector::DotProduct(MoveVec, FVector(-Info.NearNormal, 0.0f)) * (1.0f - Repulsion);

	return MovedPos;
}

FVector ASquareLand::AdjustMoveOutWater(const FVector & OldPos, FVector MovedPos, FVector & MoveVec, float XLen, float YLen, float Repulsion)
{
	FVector2D MyPos = (FVector2D)GetActorLocation();
	float MyXLen = GetXLength() * 0.5f;
	float MyYLen = GetYLength() * 0.5f;
	XLen = XLen * 0.5f;
	YLen = YLen * 0.5f;
	// 埋まっている量を計算
	float X_Left = MovedPos.X + XLen - (MyPos.X - MyXLen);
	float X_Right = MyPos.X + MyXLen - (MovedPos.X - XLen);
	float Y_Down = MovedPos.Y + YLen - (MyPos.Y - MyYLen);
	float Y_Up = MyPos.Y + MyYLen - (MovedPos.Y - YLen);

	if (!((X_Left > 0 && X_Right > 0) && (Y_Down > 0 && Y_Up > 0)))
		return MovedPos;

	// 押し出す量を計算
	FVector PushVec = FVector::ZeroVector;
	PushVec.X = (MoveVec.X > 0) ? -X_Left : X_Right;	// true:左側にいる時, false:右側にいる時
	PushVec.Y = (MoveVec.Y > 0) ? -Y_Down : Y_Up;		// true:下側にいる時, false:上側にいる時

	// 押し出す量が大きい方向は無効にする
	(FMath::Abs(PushVec.X) >= FMath::Abs(PushVec.Y)) ? PushVec.X = 0.0f : PushVec.Y = 0.0f;
	MovedPos += PushVec;

	// 反射方向を計算
	PushVec.Normalize();
	MoveVec = FVector(MyFunc::GetReflectVector2D((FVector2D)MoveVec, (FVector2D)PushVec), 0.0f);
	MoveVec -= PushVec * FVector::DotProduct(MoveVec, PushVec) * (1.0f - Repulsion);

	return MovedPos;
}

// Called when the game starts or when spawned
void ASquareLand::BeginPlay()
{
	Super::BeginPlay();
	Obb.Position = GetActorLocation();
	Obb.HorizontalVector = FVector::RightVector * XLength * 0.5f;
	Obb.VerticalVector = FVector::ForwardVector * YLength * 0.5f;
}