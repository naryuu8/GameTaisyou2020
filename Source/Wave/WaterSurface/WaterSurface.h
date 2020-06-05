// Fill out your copright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../ProceduralMesh/ProceduralMeshActor.h"
#include "WaterSurface.generated.h"

class ALandPoint;
class AFlashFlood;
class ABreakSquareLand;

enum class VertexType
{
	Water,	// 水
	Land,	// 陸
	Cliff,	// 崖
	None
};

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
	float GetMaxWaveHeight() { return MaxWaveHight; };	// 波の最大高さを取得
	float GetWaveSpeed() { return WaveSpeed; }
	FVector AdjustMoveInField(const FVector & worldPos, float circleRadius);
	FVector AdjustMoveInWater(const AActor * Object, FVector& moveVec, float circleRadius);
	FVector AdjustMoveInWater(const AActor * Object, FVector& moveVec, float XLen, float YLen);
	FVector AdjustMoveInLand(const FVector & worldPos, const FVector & moveVec, float circleRadius, const FVector & WaterCheckPos, float WaterCheckRadius);
	bool IsInWater(FVector worldPos);
	bool IsInLand(FVector worldPos);
	VertexType GetVertexType(FVector worldPos);
	bool IsInField(FVector worldPos);	// ステージ外かどうか調べる
	bool IsInField(FVector worldPos, float CircleRadius);	// ステージ外かどうか調べる
	FVector GetGetOffPos(FVector WorldPos, float Radius);	// 筏から降りれる場所を取得
	ALandPoint * GetLandPoint(const FVector & WorldPos, bool IsLand = true);	// 指定した座標に接している地面を取得
	ALandPoint * GetLandPoint(const FVector & WorldPos, float Radius, bool IsLand = true);	// 指定した座標に接している地面を取得
	FVector GetCenterPos();
	FVector GetStartPos() { return StartPoint->GetActorLocation(); };
private:
	void CreateWave(int32 x, int32 y, float pawer);
	void SetCircleLand(FVector CirclePostion, float Radius, VertexType Type);
	void SetSquareLand(FVector SquareLocation, float XLength, float YLength, VertexType Type);
	void SetLand(int X, int Y, float Z, VertexType Type);

	void TickFlashFloodWave(AFlashFlood* FlashFlood);

	void HammerBreakLand(const FVector & worldPos);

	int32 CalcIndex(int32 x, int32 y);

private:
	FIntPoint SplitPointNum = FIntPoint(0,0);

	UPROPERTY(EditAnywhere, meta = (ClampMin = "5.0", ClampMax = "100.0"))
		float SplitSpace = 10.0f;	// 頂点の間隔
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
		float WavePower = 1.0f;
	UPROPERTY(EditAnywhere)
		float MaxWaveHight = 100.0f;
	UPROPERTY(EditAnywhere)
		FLinearColor WaterColor = FLinearColor::Blue;
	UPROPERTY(EditAnywhere)
		FLinearColor WaveColor = FLinearColor::White;

	TArray<VertexType> VertexTypes;

	TArray<float> CurrentHeights;
	TArray<float> PrevHeights;
	TArray<float> NewHeights;

	TArray<ALandPoint*> LandPointActors;	// 衝突処理などに使うのでメンバにする
	TArray<AFlashFlood*> FlashFloods;		// 水流の計算に使うのでメンバにする
	TArray<ABreakSquareLand*> BreakLands;	// ハンマーで床壊すときに使う
};