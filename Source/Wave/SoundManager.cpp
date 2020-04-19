#include "SoundManager.h"

//インスタンスへのアドレス
static ASoundManager* SoundManagerInstance = nullptr;

// Sets default values
ASoundManager::ASoundManager()
{
	PrimaryActorTick.bCanEverTick = true;

	SoundManagerInstance = this;
}

void ASoundManager::BeginPlay() {
	if (AudioComponent.Num() == SoundList.Num())
	{
		return;
	}

	for (const auto& Cue : SoundList)
	{
		FString CueName = Cue->GetName();
		FName AudioCompName(*CueName);

		// create AudioComponent
		auto NewAudioComponent = NewObject<UAudioComponent>(this, AudioCompName);

		AudioComponent.Add(NewAudioComponent);
	}
}

void ASoundManager::PlaySound(int Index)
{
	if (Index < 0 || Index >= SoundList.Num()) return;//リストに存在しないインデックスなら再生しない
	if (SoundList.GetData()[Index] == nullptr) return;//サウンドが設定されていないなら再生しない

	AudioComponent[Index]->Sound = SoundList.GetData()[Index];//サウンド設定
	AudioComponent[Index]->Play();//サウンドを再生する
	AudioComponent[Index]->SetPaused(true);
}

void ASoundManager::StopSound(int Index) {
	if (Index < 0 || Index >= SoundList.Num()) return;//リストに存在しないインデックスなら再生しない
	if (SoundList.GetData()[Index] == nullptr) return;//サウンドが設定されていないなら再生しない

	AudioComponent[Index]->Stop();//サウンドを停止する
}

ASoundManager* ASoundManager::GetInstance()
{
	return SoundManagerInstance;
}

void ASoundManager::SafePlaySound(int Index)
{
	if (!SoundManagerInstance) return;
	SoundManagerInstance->PlaySound(Index);
}

void ASoundManager::SafeStopSound(int Index)
{
	if (!SoundManagerInstance) return;
	SoundManagerInstance->StopSound(Index);
}