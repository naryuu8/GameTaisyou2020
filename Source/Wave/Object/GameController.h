// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameController.generated.h"

class UHammerCountUI;
class UStageClearUI;
class UGameOverUI;
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
	//ノルマの割合 0-100で入力
	UPROPERTY(EditAnywhere, Category = "Game")
		float NormaPercent = 0;
private:
	// ゲームクリアかどうか
	bool IsGameClear;
	//ゲームオーバーかどうか
	bool IsGameOver;
	//ゴールに入った荷物をカウント
	int GoalCount;
	// シーン上の全てのゴールのポインタを所持する
	TArray<class AGoal*> GoalArray;

	// 表示するUI　エディタで指定する
	UPROPERTY(EditAnywhere)
		TSubclassOf<UStageClearUI> StageClearUIClass;
	UPROPERTY(EditAnywhere)
		TSubclassOf<UGameOverUI> GameOverUIClass;
	UStageClearUI* StageClearUI;
	UGameOverUI* GameOverUI;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// ゲームクリアかどうかを判断する関数
	UFUNCTION(BlueprintCallable, Category = "C++Library")
		FORCEINLINE bool GetIsClear() const { return IsGameClear; }
	UFUNCTION(BlueprintCallable, Category = "C++Library")
		FORCEINLINE int GetGoalCount() const { return GoalCount; }
	UFUNCTION(BlueprintCallable, Category = "C++Library")
		void SetNormaGoalCount(const int goalcount) { NormaGoalCount = goalcount; }
	UFUNCTION(BlueprintCallable, Category = "C++Library")
		FORCEINLINE int GetNormaGoalCount() const { return NormaGoalCount; }
	UFUNCTION(BlueprintCallable, Category = "C++Library")
		FORCEINLINE float GetNormaPercent() const { return NormaPercent; }
	UFUNCTION(BlueprintCallable, Category = "C++Library")
		void SetNormaPercent(const float percent) { NormaPercent = percent; }
	//データテーブルから対応するステージ番号から
	//ゲージのパーセントとクリアに必要な荷物数取得（デバッグの時は読み込まない）
	UFUNCTION(Category = "C++Event", BlueprintImplementableEvent, BlueprintCallable)
		void DataTableLoad();
private:
	// ゴール済みの個数を取得
	int GetGoalCount();
	// 爆発済みの家の個数を取得
	int GetNotExplotionCount();
	void CreateStageClearUI();
	void CreateGameOverUI();
	void InputGameOverUI();
	//ゲージのノルマ設定
	void SetNorma();
};
