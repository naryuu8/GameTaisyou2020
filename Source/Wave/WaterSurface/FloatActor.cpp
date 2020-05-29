// Fill out your copyright notice in the Description page of Project Settings.


#include "FloatActor.h"
#include "Kismet/GameplayStatics.h"
#include "WaterSurface.h"

// Sets default values
AFloatActor::AFloatActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AFloatActor::BeginPlay()
{
	Super::BeginPlay();
	
	WaterSurface = Cast<AWaterSurface>((UGameplayStatics::GetActorOfClass(GetWorld(), AWaterSurface::StaticClass())));
}

// Called every frame
void AFloatActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if ((!WaterSurface->IsInWater(GetActorLocation())))
	{
		SetActorLocation(GetActorLocation() + FVector(0, 0, -2));
		return;
	}

	// ”g‚ÌŒX‚«‚É‰ž‚¶‚ÄˆÚ“®‚·‚é
	FVector WavePower = WaterSurface->GetWavePower(GetActorLocation());
	FVector MoveVec = WavePower * FloatSpeed * 0.01f;
	Velocity += MoveVec;
	if (Velocity.Size() > MinFloatWavePower)
		SetActorLocation(WaterSurface->AdjustMoveInWater(this, Velocity, FloatScale));
	Velocity *= 0.98f;

	// ”g‚Ì‚‚³‚É‡‚í‚¹‚é
	FVector CurPos = GetActorLocation();
	float Height = WaterSurface->GetWaveHeight(CurPos);
	Height = FMath::Lerp(CurPos.Z, Height, 0.1f);
	SetActorLocation(FVector(CurPos.X, CurPos.Y, Height));
}