// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "ProceduralMeshActor.generated.h"

UCLASS()
class WAVE_API AProceduralMeshActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AProceduralMeshActor();

protected:
	virtual void BeginPlay() override;

	// メッシュの作成
	// 先に頂点だのもろもろの値を設定してから呼び出す
	virtual void CreateMesh();

	virtual void UpdateMesh();

public:	
	virtual void Tick(float DeltaTime) override;
	
protected:
	UProceduralMeshComponent* Mesh;

	// 頂点群
	TArray<FVector> Vertices;
	// インデックス群
	TArray<int32> Triangles;
	// 法線群
	TArray<FVector> Normals;
	// テクスチャー座標群
	TArray<FVector2D> UV0;
	// 頂点カラー群
	TArray<FLinearColor> VertexColors;
	// 接線群
	TArray<FProcMeshTangent> Tangents;
	// 当たり判定生成フラグ
	bool IsCreateCollision = true;
};
