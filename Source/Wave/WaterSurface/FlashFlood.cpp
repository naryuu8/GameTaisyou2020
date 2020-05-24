// Fill out your copyright notice in the Description page of Project Settings.


#include "FlashFlood.h"
#include "Runtime/Engine/Classes/Kismet//GameplayStatics.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"


// Sets default values
AFlashFlood::AFlashFlood()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFlashFlood::BeginPlay()
{
	Super::BeginPlay();
	FloatVec.Normalize();
}

// Called every frame
void AFlashFlood::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	float XOffset = XLength * 0.5f;
	float YOffset = YLength * 0.5f;
	// デバッグ用の四角を描画(高さや太さは決め打ち）
	UKismetSystemLibrary::DrawDebugLine(this, GetActorLocation() + FVector(XOffset, YOffset, 0.0f), GetActorLocation() + FVector(-XOffset, YOffset, 0.0f), FLinearColor::Blue, 0.0f, 3.0f);
	UKismetSystemLibrary::DrawDebugLine(this, GetActorLocation() + FVector(XOffset, -YOffset, 0.0f), GetActorLocation() + FVector(-XOffset, -YOffset, 0.0f), FLinearColor::Blue, 0.0f, 3.0f);
	UKismetSystemLibrary::DrawDebugLine(this, GetActorLocation() + FVector(XOffset, YOffset, 0.0f), GetActorLocation() + FVector(XOffset, -YOffset, 0.0f), FLinearColor::Blue, 0.0f, 3.0f);
	UKismetSystemLibrary::DrawDebugLine(this, GetActorLocation() + FVector(-XOffset, YOffset, 0.0f), GetActorLocation() + FVector(-XOffset, -YOffset, 0.0f), FLinearColor::Blue, 0.0f, 3.0f);
}

bool AFlashFlood::ShouldTickIfViewportsOnly() const
{
	return true;
}

FVector AFlashFlood::GetFloatVec(FVector worldPos)
{
	float xp = worldPos.X;
	float yp = worldPos.Y;
	float xs = GetActorLocation().X;
	float ys = GetActorLocation().Y;

	if (xp > xs + XLength * 0.5f) return FVector::ZeroVector;
	if (xp < xs - XLength * 0.5f) return FVector::ZeroVector;
	if (yp > ys + YLength * 0.5f) return FVector::ZeroVector;
	if (yp < ys - YLength * 0.5f) return FVector::ZeroVector;

	UE_LOG(LogTemp, Log, TEXT("Float"));
	return FloatVec * Speed;
}