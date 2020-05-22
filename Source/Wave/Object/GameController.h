// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameController.generated.h"

class UHammerCountUI;

// ゲーム中のクリア条件などを管理するクラス

UCLASS()
class WAVE_API AGameController : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGameController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// クリアかどうかのゴールのノルマカウント
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game")
		int NormaGoalCount = 0;
	//ハンマーを叩ける回数
	UPROPERTY(EditAnywhere, Category = "Game")
		int HammerCount = 0;

private:
	// ゲームクリアかどうか
	bool IsGameClear;
	// シーン上の全てのゴールのポインタを所持する
	TArray<class AGoal*> GoalArray;

	// 表示するUI　エディタで指定する
	UPROPERTY(EditAnywhere)
		TSubclassOf<UHammerCountUI> UIClass;
	// ハンマーのカウントを表示するUI
	UHammerCountUI* HammerCountUI;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// ゲームクリアかどうかを判断する関数
	UFUNCTION(BlueprintCallable, Category = "C++Library")
		FORCEINLINE bool GetIsClear() const { return IsGameClear; }

	void MinusHammerCount();

private:
	// ゴール済みの個数を取得
	int GetGoalCount();
	//ハンマーを叩ける回数が1以上ならUIを生成
	void CreateHammerCountUI();
};
