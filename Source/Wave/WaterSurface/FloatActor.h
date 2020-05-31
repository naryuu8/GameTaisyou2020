// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FloatActor.generated.h"

class AWaterSurface;

UCLASS()
class WAVE_API AFloatActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFloatActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MyDestroy();

	UPROPERTY(EditAnywhere)
		float MinFloatWavePower = 0.1f;
	UPROPERTY(EditAnywhere)
		float FloatSpeed = 0.7f;
	UPROPERTY(EditAnywhere)
		float FloatScale = 40.0f;
public:	
	FVector Velocity;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	float GetRadius() { return FloatScale; }

protected:
	AWaterSurface* WaterSurface;
	UStaticMeshComponent* StaticMeshComponent;
private:
};
