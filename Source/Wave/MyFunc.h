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
	static bool ColSegments(
		const FRay2D &seg1,          // 線分1
		const FRay2D &seg2,          // 線分2
		FVector2D* outPos = 0 // 交点（出力）
	);
};
