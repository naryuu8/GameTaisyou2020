// Fill out your copyright notice in the Description page of Project Settings.


#include "Spawner.h"

static FString ActorPaths[] =
{
	TEXT("/Game/Main/MapObject/FloatActor1_Blueprint.FloatActor1_Blueprint_C"),
	TEXT("/Game/Main/MapObject/BomBP.BomBP_C"),
};

// Sets default values
ASpawner::ASpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASpawner::BeginPlay()
{
	Super::BeginPlay();

	sc = TSoftClassPtr<AActor>(FSoftObjectPath(*ActorPaths[(int)type])).LoadSynchronous(); // パスに該当するクラスを取得

	isEnd = false;

	// 自動スポーンであればスポーンカウント開始
	if (isAutoSpawn) 
		isCounting = true;
	else				
		isCounting = false;
}

// Called every frame
void ASpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// カウントしないなら終わり
	if (!isCounting) return;

	// カウント
	currentTime += 1.0f / 60.0f;

	// 目標時間に達してなければ終了
	if (currentTime < interval) return;

	Spawn();

	if (isAutoSpawn)
	{
		currentTime -= interval;
	}
	else
	{
		isCounting = false;
		currentTime = 0;
	}
}

void ASpawner::Spawn()
{
	// 終わってれば何もしない
	if (isEnd) return;

	if (sc != nullptr)
	{
		AActor* a = GetWorld()->SpawnActor<AActor>(sc); // スポーン処理
		a->SetActorLocation(GetActorLocation()); //　座標を設定
	}

	//　ループしないのであればもうスポーンしないようにする
	if(!isLoop)isEnd = true;
}

void ASpawner::SpawnCountStart()
{
	isCounting = true;
};
