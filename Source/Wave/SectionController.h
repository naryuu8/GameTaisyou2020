// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SectionController.generated.h"

class UGoalComponent;
class AScaffold;
class AStake;

enum class ESectionState
{
	Challenge,	// 挑戦中
	Clear,		// クリア済み
	None		// 何もしない
};

UCLASS()
class WAVE_API ASectionController : public AActor
{
	GENERATED_BODY()

public:
	//　ゴール
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Section")
		TArray<AActor*> GoalActors;

	//　スイッチ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Section")
		TArray<AStake*> Stakes;

	//　足場
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Section")
		TArray<AScaffold*> Scaffolds;

	ESectionState State;

	TArray<UGoalComponent*> Goals;


protected:
	bool IsClear;
	
public:	
	// Sets default values for this actor's properties
	ASectionController();

	UFUNCTION(Category = "C++Event", BlueprintImplementableEvent, BlueprintCallable)
		void SetfromBP();

	bool GetIsClear() { return IsClear; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	bool CheckClear();

	void OnClear();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
