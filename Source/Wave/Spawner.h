// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Spawner.generated.h"

UENUM(BlueprintType)
enum class ACTOR_TYPE : uint8
{
	FLOAT_ACTOR	UMETA(DisplayName = "FlaotActor"),
	BOMB		UMETA(DisplayName = "Bomb"),
};

UCLASS()
class WAVE_API ASpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawner();

	UFUNCTION(BlueprintCallable, Category = "C++Library")
	void Spawn();

	UFUNCTION(BlueprintCallable, Category = "C++Library")
	void SpawnCountStart();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere)
		ACTOR_TYPE type;	// スポーンするBPのタイプ

	UPROPERTY(EditAnywhere)
		float interval = 1.0f;	// スポーン感覚

	UPROPERTY(EditAnywhere)
		bool isAutoSpawn = true;	 // trueなら自動でスポーン

	UPROPERTY(EditAnywhere)
		bool isLoop = true;		// スポーンを繰り返すか？(falseなら一回だけ

	bool isEnd = false;			// trueならもうスポーンしない
	bool isCounting = false;	// trueならカウントする

	float currentTime = 0;

	TSubclassOf<class AActor> sc;	// スポーンするクラスを格納
};
