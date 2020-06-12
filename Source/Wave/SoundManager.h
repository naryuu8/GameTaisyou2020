// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SoundManager.generated.h"

UENUM(BlueprintType)
enum class SOUND_TYPE : uint8
{
	TITLE_BGM		UMETA(DisplayName = "TitleBGM"),
	STAGE_BGM		UMETA(DisplayName = "StageBGM"),
	SELECT_BGM		UMETA(DisplayName = "SelectBGM"),
	CLEAR_BGM		UMETA(DisplayName = "ClearBGM"),
	OVER_BGM		UMETA(DisplayName = "OverBGM"),
	WALK_WOOD		UMETA(DisplayName = "WalkWood"),
	STAGE_SELECT	UMETA(DisplayName = "StageSelect"),
	WALK_GRASS		UMETA(DisplayName = "WalkGrass"),
	HAMMER_SMALL	UMETA(DisplayName = "HammerSmall"),
	HAMMER_MEDIUM	UMETA(DisplayName = "HammerMedium"),
	HAMMER_BIG		UMETA(DisplayName = "HammerBig"),
	HAMMER_SMALL_W	UMETA(DisplayName = "HammerSmallW"),
	HAMMER_MEDIUM_W	UMETA(DisplayName = "HammerMediumW"),
	HAMMER_BIG_W	UMETA(DisplayName = "HammerBigW"),
	GOAL			UMETA(DisplayName = "Goal"),
	EXPLOSION		UMETA(DisplayName = "Explosion"),
	FALL_ACTOR		UMETA(DisplayName = "FallActor"),
	FALL_PLAYER		UMETA(DisplayName = "FallPlayer"),
	BREAK_LAND		UMETA(DisplayName = "BreakLand"),
	MOVE_RAFT		UMETA(DisplayName = "MoveRaft"),
	WAVE			UMETA(DisplayName = "Wave"),
	SPAWN_SLIP		UMETA(DisplayName = "SpawnSlip"),
	COUNT_UP		UMETA(DisplayName = "CountUp"),
	STAMP			UMETA(DisplayName = "Stamp"),
	MENU_SELECT		UMETA(DisplayName = "MenuSelect"),
	MENU_DECISION	UMETA(DisplayName = "MenuDecision"),
	HAMMER_CHARGE	UMETA(DisplayName = "HammerChage"),
	QUOTA_TIME		UMETA(DisplayName = "QuotaTime"),
	ALL_STAMP		UMETA(DisplayName = "AllStamp"),
	TIME_CHECK		UMETA(DisplayName = "TimeCheck")
};

UCLASS()
class WAVE_API ASoundManager : public AActor
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere)
		TArray <UAudioComponent*> AudioComponent;	

	void AddAudioComponent(UAudioComponent* addComp) { AudioComponent.Add(addComp); }

public:	

	ASoundManager();

	virtual void BeginPlay() override;

	//@brief 音声を再生する
	//@param Index [in] 再生する音声のインデックス
	void PlaySound(SOUND_TYPE type);

	//@brief 再生可能かチェックを行い、可能なら音声を再生する
	//@param Index [in] 再生する音声のインデックス
	UFUNCTION(BlueprintCallable, Category = "Sound")
	static void SafePlaySound(SOUND_TYPE type);

	//指定した音声(Cue)が入っているUAudioComponentを返す
	UFUNCTION(BlueprintCallable, Category = "Sound")
	static UAudioComponent* CreateAudioComponent(SOUND_TYPE type);

	//@brief サウンドマネージャを取得する
	//@return サウンドマネージャへのアドレス
	static ASoundManager* GetInstance();
};
