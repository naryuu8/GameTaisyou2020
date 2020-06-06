// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

// 2D用のレイ
struct FRay2D
{
	FRay2D() {};
	FRay2D(FVector2D Origin, FVector2D Direction) : Origin(Origin), Direction(Direction) {};
	FRay2D(FVector Origin, FVector Direction) : Origin(Origin), Direction(Direction) {};
	FVector2D Origin;
	FVector2D Direction;
};

struct FRay2DCastInfo
{
	FVector2D NearPos;
	FVector2D NearNormal;
	FRay2D NearSideRay;
	bool IsHit = false;
	float HitDist = 0.0f;
};

// 足りない関数を補足するクラス
//----------------------------------------------------
// 共通する関数で追加したいものがあればご自由にどうぞ
//----------------------------------------------------
class UTexture2D;
class WAVE_API MyFunc
{
public:
	// クオータニオンをAxisとAngleから生成する
	static FQuat FromAxisAngleToQuaternion(const FVector & axis, float angle);
	//ファイルパスで指定したテクスチャ情報を返す
	static UTexture2D* LoadTexture2DFromAssetPath(const FName AssetPath);
	//ゲーム上にデバッグ文字表示
	//引数1 文字色 FColor::Green等で指定可能
	//引数2 メッセージ
	static void DisplayDebugMessage(const FColor DisplayColor,FString message);

	// 線分の衝突
	static bool ColSegments(const FRay2D &seg1, const FRay2D &seg2,	FVector2D* outPos = nullptr);
	// 線分と円の衝突
	static bool Check_Ray2D_VS_Circle(FRay2DCastInfo & Info, const FRay2D &Ray, const FVector2D & CirclePos, float CircleRadius);
	// 二つのベクトルの反射ベクトルを取得
	static FVector2D GetReflectVector2D(const FVector2D & A, const FVector2D & B)
	{
		return (A - 2.0f * FVector2D::DotProduct(A, B) * B);
	};

	// レイの交点を調べて距離を取得する関数
	static bool Check_Ray2D_VS_Ray2D(FRay2DCastInfo & Info, FRay2D RayA, const FRay2D & RayB, const FVector2D & Normal);
	// 適当に作ったからバグあり
	static bool Check_CircleRay2D_VS_Ray2D(FRay2DCastInfo & Info, FRay2D RayA, float CircleRadius, const FRay2D & RayB, const FVector2D & Normal);
};
