// Fill out your copyright notice in the Description page of Project Settings.


#include "TitlePlayer.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math/UnrealMathUtility.h"
// Sets default values
ATitlePlayer::ATitlePlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATitlePlayer::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATitlePlayer::BeginPlay_C()
{

}

// Called every frame
void ATitlePlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATitlePlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ATitlePlayer::TargetRotation()
{
	SetActorRotation(UKismetMathLibrary::FindLookAtRotation(FVector(GetActorLocation().X, GetActorLocation().Y,0.0f), FVector(TargetPoint->GetActorLocation().X, TargetPoint->GetActorLocation().Y, 0.0f)));
}

void ATitlePlayer::TargetMove()
{
	IsMove = true;
	if (IsTarget)return;
	FVector position = FMath::Lerp(this->GetActorLocation(), TargetPoint->GetActorLocation(), MoveSpeed);
	float z = this->GetActorLocation().Z;
	SetActorLocation(FVector(position.X, position.Y,z));
}