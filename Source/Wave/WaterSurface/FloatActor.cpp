// Fill out your copyright notice in the Description page of Project Settings.


#include "FloatActor.h"
#include "Kismet/GameplayStatics.h"
#include "WaterSurface.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"

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
	StaticMeshComponent = Cast<UStaticMeshComponent>(GetComponentByClass(UStaticMeshComponent::StaticClass()));
}

// Called every frame
void AFloatActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// メッシュがセットされていなかったら何もしない
	if (!StaticMeshComponent) return;

	// フィールドの外に出た時
	if ((!WaterSurface->IsInField(GetActorLocation())))
	{
		// 物理演算を使用する
		if (!StaticMeshComponent->IsSimulatingPhysics())
			StaticMeshComponent->SetSimulatePhysics(true);

		// 数秒後にオブジェクトを削除
		FTimerManager& timerManager = GetWorld()->GetTimerManager();
		FTimerHandle handle;
		timerManager.SetTimer(handle, this, &AFloatActor::MyDestroy, 3.0f);
		return;
	}

	// 波の傾きに応じて移動する
	FVector WavePower = WaterSurface->GetWavePower(GetActorLocation());
	FVector MoveVec = WavePower * FloatSpeed * 0.01f;
	Velocity += MoveVec;
	if (Velocity.Size() > MinFloatWavePower)
		SetActorLocation(WaterSurface->AdjustMoveInWater(this, Velocity, FloatScale));
	Velocity *= 0.98f;

	// 波の高さに合わせる
	FVector CurPos = GetActorLocation();
	float Height = WaterSurface->GetWaveHeight(CurPos);
	Height = FMath::Lerp(CurPos.Z, Height, 0.1f);

	SetActorLocation(FVector(CurPos.X, CurPos.Y, Height));
}

void AFloatActor::MyDestroy()
{
	this->Destroy();
}
