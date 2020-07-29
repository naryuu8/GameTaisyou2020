// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VersusController.generated.h"
class UHammerCountUI;
class UGameOverUI;
class UResultUI;
class UGameTimeUI;
class APlayerCharacter;
class UNimotuCountUI;
class UPauseUI;
class UFadeUI;
class UControlTipsUI;
class UBattleTimeUI;
class UBattleResultUI;
class UBattleGameStartUI;
UCLASS()
class WAVE_API AVersusController : public AActor
{
	GENERATED_BODY()
public:
	//制限時間
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game")
		int TimeLimit = 60;
	//スクリーンショット用デバッグフラグ
	UPROPERTY(EditAnywhere, Category = "Game")
		bool DebugScreenMode = false;
	//操作説明表示フラグ
	UPROPERTY(EditAnywhere, Category = "Game")
		bool IsTips = false;
	// BGM用
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game")
		class UAudioComponent* AudioComponent;
private:
	APlayerCharacter* GetPlayer1; // プレイヤー1情報
	APlayerCharacter* GetPlayer2; // プレイヤー2情報
	FVector Player1RespawnLocation;
	FVector Player2RespawnLocation;
	int Player1Score = 0;
	int Player2Score = 0;

	bool IsBatlleFinish = false;
	bool IsPause = false;
	bool IsResult = false;
	bool IsStartResultEvent = false;

	// 表示するUI　エディタで指定する
	// TODO::バトル用リザルトUI,バトル用時間UI,バトル用ポーズUIが必要
	UPROPERTY(EditAnywhere)
		TSubclassOf<UFadeUI> FadeUIClass;
	UPROPERTY(EditAnywhere)
		TSubclassOf<UControlTipsUI> ControlTipsUIClass;
	UPROPERTY(EditAnywhere)
		TSubclassOf<UBattleTimeUI> BattleTimeUIClass = nullptr;
	UPROPERTY(EditAnywhere)
		TSubclassOf<UBattleResultUI> BattleResultUIClass = nullptr;
	UPROPERTY(EditAnywhere)
		TSubclassOf<UBattleGameStartUI> BattleGameStartUIClass = nullptr;

	//勝手に開放されることがあるのでガベージコレクションの対象外にする
	UPROPERTY()
		UPauseUI* PauseUI = nullptr;
	UPROPERTY()
		UBattleResultUI* BattleResultUI = nullptr;
	UPROPERTY()
		UGameTimeUI* GameTimeUI = nullptr;
	UPROPERTY()
		UGameOverUI* GameOverUI = nullptr;
	UPROPERTY()
		UResultUI* ResultUI = nullptr;
	UPROPERTY()
		UControlTipsUI* ControlTipsUI = nullptr;
	UPROPERTY()
		UBattleGameStartUI* BattleGameStartUI = nullptr;
	UPROPERTY()
		UFadeUI* FadeUI = nullptr;
public:
	UPROPERTY(BlueprintReadWrite, Category = "C++Class")
		UBattleTimeUI* BattleTimeUI = nullptr;
	UPROPERTY(BlueprintReadWrite, Category = "C++Class")
		bool IsTimeOver = false;
	UFUNCTION(BlueprintCallable, Category = "C++Library")
		float GetNowTimeAngle();

	UFUNCTION(BlueprintCallable, Category = "C++Library")
		FORCEINLINE int GetTimeLimit() const { return TimeLimit; }

	//制限時間をセット
	UFUNCTION(BlueprintCallable, Category = "C++Library")
		void SetTime(const int time) { TimeLimit = time; }

	//タイムカウントアニメが再生されていたら一時停止する
	void SetTimeCountPause();
	//タイムカウントアニメが止まっていたら続きから再生する
	void SetTimeCountRePlay();

	// ポーズ呼び出し
	UFUNCTION(BlueprintCallable, Category = "C++Library")
		void PauseCall();

	//　ポーズやリザルト時に必要な入力関数
	UFUNCTION(BlueprintCallable, Category = "C++Library")
		void InputRightCall();
	UFUNCTION(BlueprintCallable, Category = "C++Library")
		void InputLeftCall();
	UFUNCTION(BlueprintCallable, Category = "C++Library")
		void InputSelectCall();

	UFUNCTION(BlueprintCallable, Category = "C++Library")
		void StopBGM();
	//時間カウントタイムラインを開始
	UFUNCTION(Category = "C++Event", BlueprintImplementableEvent, BlueprintCallable)
		void StartTimeCount();
private:

	// 操作説明UIを作成
	void CreateControlTipsUI();
	//時間UIを作成
	void CreateBattleTimeUI();
	//リザルトUIを作成
	void CreateBattleResultUI();
	//開始時のカウントダウン作成
	void CreateBattleGameStartUI();
	//ゲーム開始時のフェードアウト
	void InitFadeOut();
	//時計の針を進める
	void UpdateTime();
	//カウントダウンの残り時間0の時trueを返す
	bool GetLimitTimeZero();
	//表示しているUIを非表示にする
	void RemoveUI();
	// ステージ上のアイコンを非表示
	void SetAllInvisibleStageIcon();

	// プレイヤーが落ちたかを判定してスコアやリスポーンの処理を行う
	void CheckPlayerFall();
	// バトルを終了する
	void BattleFinish();
public:
	// セクションの登録
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Versus")
		TArray<TSubclassOf<class ASectionController>> Sections;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Versus")
		float SectionCount = 5;

	void Goal(int BattleNumber);

private:
	TArray<ASectionController*> Player1Sections;
	TArray<ASectionController*> Player2Sections;

	float SectionSize = 2000;
	//リザルトUIを作成
	//勝敗が決定したら呼ぶ
	void InputBattleResultUI();
public:	
	// Sets default values for this actor's properties
	AVersusController();
	void CreateSections();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void AddSection(int index, int PlayerNumber, int Cnt);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
