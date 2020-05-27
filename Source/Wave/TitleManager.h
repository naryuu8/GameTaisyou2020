// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TitleManager.generated.h"
enum class ETitleState
{
	Title,
	TitleMove,
	StageSelect,
};
class ATitleCamera;
class ATitlePlayer;
class UTitleUI;
class UFadeUI;
UCLASS()
class WAVE_API ATitleManager : public APawn
{
	GENERATED_BODY()
private:
	//カメラへのアドレス
	UPROPERTY(EditAnywhere) 
		AActor* TitleCamera = nullptr;
	UPROPERTY(EditAnywhere)
		AActor* TitleMoveCamera = nullptr;
	UPROPERTY(EditAnywhere)
		ATitleCamera* StageSelectCamera = nullptr;
	UPROPERTY(EditAnywhere)
		ATitlePlayer* TitlePlayer = nullptr;
	UPROPERTY(EditAnywhere)
		TSubclassOf<UFadeUI> FadeUIClass;
	UPROPERTY(EditAnywhere)
		TSubclassOf<UTitleUI> TitleUIClass = nullptr;
	UPROPERTY(EditAnywhere, Category = "Instanced")
		float TitleMoveTime = 1.0f;//タイトル画面からセレクト画面中継カメラへの時間
	UPROPERTY(EditAnywhere, Category = "Instanced")
		float StageSelectTime = 1.0f;//セレクト画面中継からセレクト画面への時間
	ETitleState State;
	int MoveFrameTime;//カメラ移動時間をフレーム時間に変換
	int MoveFrameCount;
	bool IsNoInput;//true時キー入力を無効にする
	UTitleUI* TitleUI;
private:
	void TitleInput();
	//移動先のカメラと移行スピードを設定
	void SetCameraMove(AActor* camera, const float camera_speed);
	//タイトルでボタンを押したときステージセレクトまでのカメラ遷移を行う
	void CameraMoveCheck();
public:
	// Sets default values for this pawn's properties
	ATitleManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
