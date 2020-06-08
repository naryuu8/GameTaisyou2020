// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameController.generated.h"

class UHammerCountUI;
class UGameOverUI;
class UResultUI;
class UGameTimeUI;
class APlayerCharacter;
class UNimotuCountUI;
class UPauseUI;
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
	//制限時間
	UPROPERTY(EditAnywhere, Category = "Game")
		int TimeLimit = 60;
	//ノルマ時間
	UPROPERTY(EditAnywhere, Category = "Game")
		int NormaTime = 10;
	//カウントダウン表示開始時間
	UPROPERTY(EditAnywhere, Category = "Game")
		int CountDownTime = 10;
	// BGM用
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game")
		UAudioComponent* AudioComponent;
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

	// 表示するUI　エディタで指定する
	UPROPERTY(EditAnywhere)
		TSubclassOf<UGameTimeUI> GameTimeUIClass;
	UPROPERTY(EditAnywhere)
		TSubclassOf<UGameOverUI> GameOverUIClass;
	UPROPERTY(EditAnywhere)
		TSubclassOf<UResultUI> ResultUIClass;
	UPROPERTY(EditAnywhere)
		TSubclassOf<UNimotuCountUI> NimotuCountUIClass;
	UPROPERTY(EditAnywhere)
		TSubclassOf<UPauseUI> PauseUIClass;
	UPauseUI* PauseUI = nullptr;
	UGameTimeUI* GameTimeUI = nullptr;
	UGameOverUI* GameOverUI = nullptr;
	UResultUI* ResultUI = nullptr;
	UNimotuCountUI* NimotuCountUI = nullptr;
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
		float GetNowTimeAngle();
	UFUNCTION(BlueprintCallable, Category = "C++Library")
		float GetNormaTimeAngle();
	UFUNCTION(BlueprintCallable, Category = "C++Library")
		FORCEINLINE int GetNormaTime() const { return NormaTime; }
	UFUNCTION(BlueprintCallable, Category = "C++Library")
		FORCEINLINE int GetTimeLimit() const { return TimeLimit; }
	UFUNCTION(BlueprintCallable, Category = "C++Library")
		void SetNormaTime(const int time) { NormaTime = time; }
	//制限時間とノルマ時間をセット
	UFUNCTION(BlueprintCallable, Category = "C++Library")
		void SetTime(const int time, const int norma) { TimeLimit = time; NormaTime = norma; }
	//ゲームオーバーUIを生成していたらtrue
	bool GetIsGameOverUI();
	void MinusGameMaxNimotu();
	//データテーブルから対応するステージ番号から
	//ゲージのパーセントとクリアに必要な荷物数取得（デバッグの時は読み込まない）
	UFUNCTION(Category = "C++Event", BlueprintImplementableEvent, BlueprintCallable)
		void DataTableLoad();
	//タイムカウントアニメが再生されていたら一時停止する
	void SetTimeCountPause();
	//タイムカウントアニメが止まっていたら続きから再生する
	void SetTimeCountRePlay();
	void AddGoalCount();
	void MinusGoalCount();
	void AddNotExplotionCount() { NotExplotionCount++; }
	void MinusNotExplotionCount() { NotExplotionCount--; }
private:

	void CreateGameTimeUI();
	void CreateGameOverUI();
	void CreateResultUI();
	void CreateNimotuCountUI();
	void InputGameOverUI();
	void InputResultUI();
	void InputPause();
	//時計の針を進める
	void UpdateTime();
	//ゲームクリア条件確認
	void GameClearCheck();
	//ゲームオーバー条件確認
	void GameOverCheck();
	void GameClear();
	void GameOver();
	//ゲームプレイ中の荷物数確認
	int CountGameNimotu();
	//カウントダウンの残り時間0の時trueを返す
	bool GetLimitTimeZero();
};
