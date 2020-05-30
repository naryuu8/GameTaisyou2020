// Fill out your copright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../ProceduralMesh/ProceduralMeshActor.h"
#include "WaterSurface.generated.h"

class ALandPoint;

UCLASS()
class WAVE_API AWaterSurface : public AProceduralMeshActor
{
	GENERATED_BODY()

public:
	AWaterSurface();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	void AddPower(FVector worldPos, float pawer);
	FVector GetWavePower(const FVector & worldPos);
	float GetWaveHeight(const FVector & worldPos);	// 波の高さを取得
	float GetWaveSpeed() { return WaveSpeed; }
	FVector GetOutLandPos(FVector worldPos, float circleRadius);
	FVector AdjustMoveInWater(const AActor * Object, FVector& moveVec, float circleRadius);
	FVector AdjustMoveInLand(const FVector & worldPos, const FVector & moveVec, float circleRadius, const FVector & WaterCheckPos, float WaterCheckRadius);
	bool IsInWater(FVector worldPos);
	bool IsLand(FVector worldPos);
	FVector GetGetOffPos(FVector WorldPos, float Radius);
	ALandPoint * GetLandPoint(const FVector & WorldPos);	// 指定した座標に接している地面を取得

private:
	void CreateWave(int32 x, int32 y, float pawer);
	void SetCircleLand(FVector CirclePostion, float Radius);
	void SetSquareLand(FVector SquareLocation, float XLength, float YLength);

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
		float Decay = 0.3f;
	UPROPERTY(EditAnywhere)
		float WaveSpeed = 10.0f;
	UPROPERTY(EditAnywhere)
		float MaxWaveHight = 100.0f;
	UPROPERTY(EditAnywhere)
		FLinearColor WaterColor = FLinearColor::Blue;
	UPROPERTY(EditAnywhere)
		FLinearColor WaveColor = FLinearColor::White;

	TArray<bool> IsLands;

	TArray<float> CurrentHeights;
	TArray<float> PrevHeights;
	TArray<float> NewHeights;

	TArray<ALandPoint*> LandPointActors;	// 衝突処理などに使うのでメンバにする
};