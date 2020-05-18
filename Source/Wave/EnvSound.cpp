// Fill out your copyright notice in the Description page of Project Settings.


#include "EnvSound.h"

// Sets default values
AEnvSound::AEnvSound()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnvSound::BeginPlay()
{
	Super::BeginPlay();
	Play3DSound();
}

// Called every frame
void AEnvSound::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnvSound::Play3DSound()
{
	UGameplayStatics::PlaySoundAtLocation(GetWorld(),soundBase, GetActorLocation());
}
