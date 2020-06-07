// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SoundManager.generated.h"

enum class SOUND_TYPE
{
	TITLE_BGM,
	WALK_WOOD,
	STAGE_SELECT,
	WALK_GRASS,
	HAMMER_SMALL,
	HAMMER_MEDIUM,
	HAMMER_BIG,
	GOAL,
	EXPLOSION,
	FALL_ACTOR,
	FALL_PLAYER,
	BREAK_LAND,
	MOVE_RAFT,
	WAVE,
	SPAWN_SLIP,
	COUNT_UP,
	MENU_SELECT,
	MENU_DECISION
};

UCLASS()
class WAVE_API ASoundManager : public AActor
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere)
		TArray <UAudioComponent*> AudioComponent;	

public:	

	ASoundManager();

	virtual void BeginPlay() override;

	//@brief 音声を再生する
	//@param Index [in] 再生する音声のインデックス
	void PlaySound(SOUND_TYPE type);

	//@brief 再生可能かチェックを行い、可能なら音声を再生する
	//@param Index [in] 再生する音声のインデックス
	static void SafePlaySound(SOUND_TYPE type);

	//指定した音声(Cue)が入っているUAudioComponentを返す
	static UAudioComponent* CreateAudioComponent(SOUND_TYPE type);

	//@brief サウンドマネージャを取得する
	//@return サウンドマネージャへのアドレス
	static ASoundManager* GetInstance();
};
