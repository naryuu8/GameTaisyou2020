// Fill out your copyright notice in the Description page of Project Settings.


#include "CircleLand.h"
#include "Runtime/Engine/Classes/Kismet//GameplayStatics.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"

void ACircleLand::DebugDraw()
{
	// デバッグ用のサークルを描画(高さや太さは決め打ち）
	UKismetSystemLibrary::DrawDebugCylinder(this, GetActorLocation(), GetActorLocation() - FVector(0,0,10), Radius, 64, LAND_TYPE_COLORS[GetLandTypeNumber()], 0.0f, 3.0f);
}