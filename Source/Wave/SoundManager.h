// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SoundManager.generated.h"



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
	void PlaySound(TCHAR* fileName);

	//@brief 再生可能かチェックを行い、可能なら音声を再生する
	//@param Index [in] 再生する音声のインデックス
	static void SafePlaySound(TCHAR* fileName);

	//指定した音声(Cue)が入っているUAudioComponentを返す
	static UAudioComponent* CreateAudioComponent(TCHAR* fileName);

	//@brief サウンドマネージャを取得する
	//@return サウンドマネージャへのアドレス
	static ASoundManager* GetInstance();



};
