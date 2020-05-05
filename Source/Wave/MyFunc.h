// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

// 足りない関数を補足するクラス
//----------------------------------------------------
// 共通する関数で追加したいものがあればご自由にどうぞ
//----------------------------------------------------

class WAVE_API MyFunc
{
public:
	// クオータニオンをAxisとAngleから生成する
	static FQuat FromAxisAngleToQuaternion(const FVector & axis, float angle);
};
