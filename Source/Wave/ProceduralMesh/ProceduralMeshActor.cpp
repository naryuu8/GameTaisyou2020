// Fill out your copyright notice in the Description page of Project Settings.


#include "ProceduralMeshActor.h"

// Sets default values
AProceduralMeshActor::AProceduralMeshActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// mesh の中身を作る。
	Mesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("generated-mesh"));
	// とりあえず RootComponent として mesh をつっこむ。
	RootComponent = Mesh;
}

// Called when the game starts or when spawned
void AProceduralMeshActor::BeginPlay()
{
	Super::BeginPlay();
}

void AProceduralMeshActor::CreateMesh()
{
	// UProceduralMeshComponent::CreateMeshSection_LinearColor でメッシュを生成。
	// 第1引数: セクション番号; 0, 1, 2, ... を与える事で1つの UProceduralMeshComponent に複数のメッシュを内部的に同時に生成できます。
	// 第2引数: 頂点群
	// 第3引数: インデックス群
	// 第4引数: 法線群
	// 第5引数: テクスチャー座標群
	// 第6引数: 頂点カラー群
	// 第7引数: 法線群
	// 第8引数: コリジョン生成フラグ
	Mesh->CreateMeshSection_LinearColor(0, Vertices, Triangles, Normals, UV0, VertexColors, Tangents, IsCreateCollision);
}

void AProceduralMeshActor::UpdateMesh()
{
	Mesh->UpdateMeshSection_LinearColor(0, Vertices, Normals, UV0, VertexColors, Tangents);
}

// Called every frame
void AProceduralMeshActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

