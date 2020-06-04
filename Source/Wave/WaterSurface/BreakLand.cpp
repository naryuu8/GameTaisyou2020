// Fill out your copyright notice in the Description page of Project Settings.


#include "BreakLand.h"
#include "../WaterSurface/FloatActor.h"

void ABreakLand::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AFloatActor* OtherFloat = Cast<AFloatActor>(OtherActor);
	// 荷物以外は判定しない
	if (GetIsUse()) return; //すでに壊れているから
	if (!OtherFloat) return;

	// 衝突したアクターが爆弾の時爆発してゴール済みでもゴールしていないことにする
	if (OtherFloat->ActorHasTag("Bom"))
	{
		Break();
		OtherFloat->Destroy();
	}
}

void ABreakLand::Break()
{
	SetIsUse(false);
}