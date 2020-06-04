// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameController.generated.h"

class UHammerCountUI;
class UGameOverUI;
class UResultUI;
class UTimeCountUI;
class APlayerCharacter;
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
	//ハンマーが0になった時の残り時間
	UPROPERTY(EditAnywhere, Category = "Game")
		int LimitTime = 10;
private:
	APlayerCharacter* GetPlayer;//プレイヤー情報
	// ゲームクリアかどうか
	bool IsGameClear;
	//ゲームオーバーかどうか
	bool IsGameOver;
	//ゴールに入った荷物をカウント
	int GoalCount = 0;
	//壊れていない家の数をカウント
	int NotExplotionCount = 0;
	//このステージのゴール条件に入る荷物の数
	int MaxNimotu = 0;
	//ゲーム上に配置されている荷物の数（リアルタイム更新）
	int GameMaxNimotu = 0;
	// シーン上の全てのゴールのポインタを所持する
	TArray<class AGoal*> GoalArray;

	// 表示するUI　エディタで指定する
	UPROPERTY(EditAnywhere)
		TSubclassOf<UTimeCountUI> TimeCountUIClass;
	UPROPERTY(EditAnywhere)
		TSubclassOf<UGameOverUI> GameOverUIClass;
	UPROPERTY(EditAnywhere)
		TSubclassOf<UResultUI> ResultUIClass;
	UTimeCountUI* TimeCountUI = nullptr;
	UGameOverUI* GameOverUI = nullptr;
	UResultUI* ResultUI = nullptr;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// ゲームクリアかどうかを判断する関数
	UFUNCTION(BlueprintCallable, Category = "C++Library")
		FORCEINLINE bool GetIsClear() const { return IsGameClear; }
	UFUNCTION(BlueprintCallable, Category = "C++Library")
		FORCEINLINE bool GetIsGameOver() const { return IsGameOver; }
	UFUNCTION(BlueprintCallable, Category = "C++Library")
		FORCEINLINE int GetGoalCount() const { return GoalCount; }
	UFUNCTION(BlueprintCallable, Category = "C++Library")
		void SetNormaGoalCount(const int goalcount) { NormaGoalCount = goalcount; }
	UFUNCTION(BlueprintCallable, Category = "C++Library")
		FORCEINLINE int GetNormaGoalCount() const { return NormaGoalCount; }
	UFUNCTION(BlueprintCallable, Category = "C++Library")
		void SetMaxNimotu(const int num) { MaxNimotu = num; };
	UFUNCTION(BlueprintCallable, Category = "C++Library")
		int GetMaxNimotu();
	UFUNCTION(BlueprintCallable, Category = "C++Library")
		FORCEINLINE float GetNormaPercent() const { return NormaPercent; }
	UFUNCTION(BlueprintCallable, Category = "C++Library")
		void SetNormaPercent(const float percent) { NormaPercent = percent; }
	void MinusGameMaxNimotu() { GameMaxNimotu--; }
	//データテーブルから対応するステージ番号から
	//ゲージのパーセントとクリアに必要な荷物数取得（デバッグの時は読み込まない）
	UFUNCTION(Category = "C++Event", BlueprintImplementableEvent, BlueprintCallable)
		void DataTableLoad();
	//タイムカウントアニメが再生されていたら一時停止する
	void SetTimeCountPause();
	//タイムカウントアニメが止まっていたら続きから再生する
	void SetTimeCountRePlay();
	void AddGoalCount() { GoalCount++; }
	void MinusGoalCount() { GoalCount--; }
	void AddNotExplotionCount() { NotExplotionCount++; }
	void MinusNotExplotionCount() { NotExplotionCount--; }
private:
	// ゴール済みの個数を取得
	int GetGoalCount();
	// 爆発済みの家の個数を取得
	int GetNotExplotionCount();
	void CreateTimeCountUI();
	void CreateGameOverUI();
	void CreateResultUI();
	void InputGameOverUI();
	void InputResultUI();
	//ゲージのノルマ設定
	void SetNorma();
	//ゲームクリア条件確認
	void GameClearCheck();
	//ゲームオーバー条件確認
	void GameOverCheck();
	//ゲームプレイ中の荷物数確認
	int CountGameNimotu();
	//ハンマーのHPが0でハンマーを叩き終わったらカウントダウンを開始する
	void CheckTimeCount();
	//カウントダウンの残り時間0の時trueを返す
	bool GetLimitTimeZero();
};
