// Fill out your copright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../ProceduralMesh/ProceduralMeshActor.h"
#include "WaterSurface.generated.h"

class ACircleLandPoint;

UCLASS()
class WAVE_API AWaterSurface : public AProceduralMeshActor
{
	GENERATED_BODY()

public:
	AWaterSurface();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	void AddPower(FVector worldPos, float pawer);
	FVector GetWavePower(FVector worldPos);
	float GetWaveSpeed() { return WaveSpeed; }
	FVector GetOutLandPos(FVector worldPos, float circleRadius);

private:
	void CreateWave(int32 x, int32 y, float pawer);
	void SetCircleLand(FVector CirclePostion, float Radius);
	void SetSquareLand(FVector SquareLocation, float XLength, float YLength);
	void SetLand(int32 sx, int32 sy, int32 ex, int32 ey);

	int32 CalcIndex(int32 x, int32 y);
	FVector2D LocationToVertices(FVector Location);

private:
	float X_Size;
	float Y_Size;

	UPROPERTY(EditAnywhere)
		FVector2D SplitVector = FVector2D(100,100);
	UPROPERTY(EditAnywhere)
		AActor* StartPoint;
	UPROPERTY(EditAnywhere)
		AActor* EndPoint;
	UPROPERTY(EditAnywhere)
		UMaterialInterface *Material;
	UPROPERTY(EditAnywhere)
		float Decay = 0.1f;
	UPROPERTY(EditAnywhere)
		float WaveSpeed = 1.0f;

	TArray<bool> IsLand;

	TArray<float> CurrentHeights;
	TArray<float> PrevHeights;
	TArray<float> NewHeights;
};