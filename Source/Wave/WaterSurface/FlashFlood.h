// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FlashFlood.generated.h"

UCLASS()
class WAVE_API AFlashFlood : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFlashFlood();

	float GetXLength() { return XLength; }
	float GetYLength() { return YLength; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual bool ShouldTickIfViewportsOnly() const override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FVector GetFloatVec(FVector worldPos);

	float GetCurrentTime() { return CurrentTime; }

public:
	UPROPERTY(EditAnywhere)
		float SwingWight = 0.01f;
	UPROPERTY(EditAnywhere)
		float MaxHight = 30.0f;
	UPROPERTY(EditAnywhere)
		float XLength = 100.0f;
	UPROPERTY(EditAnywhere)
		float YLength = 100.0f;
	UPROPERTY(EditAnywhere)
		float Speed = 100.0f;
	UPROPERTY(EditAnywhere)
		FVector FloatVec;

private:
	float CurrentTime = 0.0f; // ŽžŠÔŒv‘ª—p
};
