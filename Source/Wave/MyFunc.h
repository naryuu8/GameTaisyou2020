// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

// 足りない関数を補足するクラス

class WAVE_API MyFunc
{
public:
	// クオータニオンをAxisとAngleから生成する
	static FQuat SetAxisAngle(const FVector & axis, float angle);
};
