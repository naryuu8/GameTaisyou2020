// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SoundManager.generated.h"

UENUM(BlueprintType)
enum class SoundData : uint8 {
	MT_Explosion UMETA(DisplaName = "SoundCue'/Game/Main/Sound/hammer_sougen_Cue.hammer_sougen_Cue'"),
};

UCLASS()
class WAVE_API ASoundManager : public AActor
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere)
		TArray <UAudioComponent*> AudioComponent;
	
	UPROPERTY(EditAnywhere, category = "SoundSetList")
	TArray <USoundBase*> SoundList;//再生する音声のリスト
public:	

	ASoundManager();

	virtual void BeginPlay() override;

	//@brief 音声を再生する
	//@param Index [in] 再生する音声のインデックス
	void PlaySound(FString fileName);

	//@brief 音声を再生する
	//@param Index [in] 再生する音声のインデックス
	void Play3DSound(int Index, UWorld* world, FVector vector);

	//@brief 音声を停止する
	//@param Index [in] 停止する音声のインデックス
	void StopSound(int Index);

	//@brief 再生可能かチェックを行い、可能なら音声を再生する
	//@param Index [in] 再生する音声のインデックス
	static void SafePlaySound(int Index);

	//@brief 停止可能かチェックを行い、可能なら音声を停止する
	//@param Index [in] 停止する音声のインデックス
	static void SafeStopSound(int Index);

		
	static void SafePlay3DSound(int Index, UWorld* world, FVector vector);

	static UAudioComponent* CreateAudioComponent();

	//@brief サウンドマネージャを取得する
	//@return サウンドマネージャへのアドレス
	static ASoundManager* GetInstance();



};
