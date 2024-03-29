// Fill out your copyright notice in the Description page of Project Settings.


#include "Scaffold.h"
#include "WaterSurface/WaterSurface.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
AScaffold::AScaffold()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SetIsUse(true);
}

// Called when the game starts or when spawned
void AScaffold::BeginPlay()
{
	Super::BeginPlay();
	
	AppearZ = GetActorLocation().Z;

	SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y, SinkZ));

	IsClear = false;
	IsFinish = false;

	TArray<AActor*> WaterSurface;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWaterSurface::StaticClass(), WaterSurface);
	for (auto Actor : WaterSurface)
	{
		AWaterSurface * Water = Cast<AWaterSurface>(Actor);
		if (!Water)continue;
		SetIsUse(false);
		Water->SetSquareLand(GetActorLocation(), XLength, YLength, VertexType::Water);
	}
	IsLand = false;
}

// Called every frame
void AScaffold::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsFinish) return;

	Rise(DeltaTime);
}

void AScaffold::Rise(float DeltaTime)
{
	if (!IsClear) return;

	// 移動量を計算して移動
	FVector MovedLocation = GetActorLocation();
	MovedLocation.Z += +((AppearZ - SinkZ) / RiseTime) * DeltaTime;
	if (MovedLocation.Z >= AppearZ)
	{
		MovedLocation.Z = AppearZ;
		IsFinish = true;
	}

	SetActorLocation(MovedLocation);
}

void AScaffold::Clear()
{
	IsClear = true;

	SetIsUse(true);

	AActor* WaterActor = UGameplayStatics::GetActorOfClass(GetWorld(), AWaterSurface::StaticClass());
	if (!WaterActor) return;

	AWaterSurface* WaterSurface = (AWaterSurface*)WaterActor;
	if (!WaterSurface) return;
	
	UE_LOG(LogTemp, Log, TEXT("LOG_TEST"));

	SetIsUse(false);
	IsLand = true;
	WaterSurface->SetSquareLand(GetActorLocation(), XLength, YLength, VertexType::Land);
}
