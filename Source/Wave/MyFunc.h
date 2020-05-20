// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

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
	void DisplayDebugMessage(const FColor DisplayColor,FString message);
};
