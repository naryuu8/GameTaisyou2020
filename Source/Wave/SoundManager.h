// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Runtime/Engine/Classes/Components/AudioComponent.h"
#include "SoundManager.generated.h"

UCLASS()
class WAVE_API ASoundManager : public AActor
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere)
	UAudioComponent* AudioComponent = nullptr;//音声コンポーネントクラス

	UPROPERTY(EditAnywhere, category = "SoundSetList")
	TArray <USoundBase*> SoundList;//再生する音声のリスト
public:	

	ASoundManager();

	//@brief 音声を再生する
	//@param Index [in] 再生する音声のインデックス
	void PlaySound(int Index);

	//@brief 再生可能かチェックを行い、可能なら音声を再生する
	//@param Index [in] 再生する音声のインデックス
	static void SafePlaySound(int Index);

	//@brief サウンドマネージャを取得する
	//@return サウンドマネージャへのアドレス
	static ASoundManager* GetInstance();
};
