// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LandPoint.generated.h"

UENUM()
enum class ELandType
{
	Grass,
	Num	UMETA(Hidden)
};

static const FColor LAND_TYPE_COLORS[] =
{
	FColor::Green,
	FColor::Transparent
};

UCLASS()
class WAVE_API ALandPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALandPoint();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual bool ShouldTickIfViewportsOnly() const override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void DebugDraw() {};

	int GetLandTypeNumber() { return static_cast<int>(LandType.GetValue()); }
	
private:
	UPROPERTY(EditAnywhere)
		TEnumAsByte<ELandType> LandType = ELandType::Grass;
};
