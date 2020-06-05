// Fill out your copyright notice in the Description page of Project Settings.


#include "BreakSquareLand.h"

#include "Kismet/GameplayStatics.h"
#include "../WaterSurface/WaterSurface.h"
#include "../WaterSurface/FloatActor.h"

// Sets default values
ABreakSquareLand::ABreakSquareLand()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Scene = CreateDefaultSubobject <USceneComponent>(TEXT("Scene"));
	RootComponent = Scene;
	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BreakLandCollsion"));
	BoxComp->SetupAttachment(RootComponent);

}

void ABreakSquareLand::BeginPlay()
{
	Super::BeginPlay();
	SetIsUse(true);
	BoxComp->OnComponentBeginOverlap.AddUniqueDynamic(this, &ABreakSquareLand::OnOverlapBegin);
}

void ABreakSquareLand::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AFloatActor* OtherFloat = Cast<AFloatActor>(OtherActor);
	// 荷物以外は判定しない
	if (!GetIsUse()) return; //すでに壊れているから
	if (!OtherFloat) return;

	// 衝突したアクターが爆弾の時爆発してゴール済みでもゴールしていないことにする
	if (OtherFloat->ActorHasTag("Bom"))
	{
		Break();
		OtherFloat->Destroy();
	}
}

void ABreakSquareLand::Break()
{
	BreakAnime();
	TArray<AActor*> WaterSurface;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWaterSurface::StaticClass(), WaterSurface);
	for (auto Actor : WaterSurface)
	{
		AWaterSurface * Water = Cast<AWaterSurface>(Actor);
		if (!Water)continue;
		Water->SetSquareLand(GetActorLocation(), XLength, YLength, VertexType::Water);
	}
	SetIsUse(false);
}