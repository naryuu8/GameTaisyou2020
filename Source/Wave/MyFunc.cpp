// Fill out your copyright notice in the Description page of Project Settings.


#include "MyFunc.h"
#include "Engine/Texture2D.h"

// Ž²( axis ) ‰ñ“]Šp“x( angle( ƒ‰ƒWƒAƒ“Šp ) )
FQuat MyFunc::FromAxisAngleToQuaternion(const FVector & axis, float angle)
{
	FQuat q;
	float t = FMath::Sin(angle / 2);
	q.X = axis.X * t;
	q.Y = axis.Y * t;
	q.Z = axis.Z * t;
	q.W = FMath::Cos(angle / 2);

	return q;
}

UTexture2D* MyFunc::LoadTexture2DFromAssetPath(const FName AssetPath)
{
	return  LoadObject<UTexture2D>(NULL, *AssetPath.ToString(), NULL, LOAD_None, NULL);
}
