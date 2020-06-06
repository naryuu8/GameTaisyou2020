// Fill out your copyright notice in the Description page of Project Settings.


#include "BreakFloatActor.h"
#include "../WaterSurface/FloatActor.h"

ABreakFloatActor::ABreakFloatActor()
{
	PrimaryActorTick.bCanEverTick = true;
	Scene = CreateDefaultSubobject <USceneComponent>(TEXT("Scene"));
	RootComponent = Scene;
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("BreakFloatActorCollsion"));
	SphereComp->SetupAttachment(RootComponent);
}

void ABreakFloatActor::BeginPlay()
{
	Super::BeginPlay();
	IsExplotion = false;
	SphereComp->OnComponentBeginOverlap.AddUniqueDynamic(this, &ABreakFloatActor::OnOverlapBegin);
}

// Called every frame
void ABreakFloatActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABreakFloatActor::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AFloatActor* OtherFloat = Cast<AFloatActor>(OtherActor);
	// ‰×•¨ˆÈŠO‚Í”»’è‚µ‚È‚¢
	if (IsExplotion) return; //‚·‚Å‚É‰ó‚ê‚Ä‚¢‚é‚©‚ç
	if (!OtherFloat) return;

	if (OtherFloat->ActorHasTag("Bom"))
	{
		IsExplotion = true;
		SetDeth();
		BreakAnime();
		OtherFloat->Destroy();
	}
}