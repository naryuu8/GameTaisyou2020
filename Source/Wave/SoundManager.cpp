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

	auto NewAudioComponent = NewObject<UAudioComponent>(this);
//	UE_LOG(LogTemp, Log, TEXT("SoundCue'/Game/Main/Sound/hammer_sougen_Cue.hammer_sougen_Cue'"));
	//UE_LOG(LogTemp, Log,SoundData::MT_Explosion);
	FName s = "SoundCue'/Game/Main/Sound/hammer_sougen_Cue.hammer_sougen_Cue'";
	NewAudioComponent->Sound = LoadObject<USoundBase>(NULL,TEXT("SoundCue'/Game/Main/Sound/hammer_sougen_Cue.hammer_sougen_Cue'"), NULL, LOAD_None, NULL);
//	UE_LOG(LogTemp, Log, TEXT("Log...= %s"), static_cast<TCHAR>(SoundData::MT_Explosion));
	AudioComponent.Add(NewAudioComponent);

}

void ASoundManager::PlaySound(FString fileName)
{
//	if (SoundList.GetData()[Index] == nullptr) return;//サウンドが設定されていないなら再生しない

//	USoundBase* sound;
	for (int i = 0; i < AudioComponent.Num(); i++) {
		if (!AudioComponent[i]->IsPlaying()) {
//			FString s = FString::FromInt(static_cast<int>(SoundData::MT_Explosion));

			AudioComponent[i]->Sound = LoadObject<USoundBase>(NULL,static_cast<TCHAR>(SoundData::MT_Explosion), NULL, LOAD_None, NULL);

//			static ConstructorHelpers::FObjectFinder< USoundBase >sound(TEXT("SoundCue'/Main/Sound/SE/enter_se_Cue'"));

		}
	//		AudioComponent[i]->Play();
	}
//	AudioComponent[Index]->Sound = SoundList.GetData()[Index];//サウンド設定
//	AudioComponent[Index]->Play();//サウンドを再生する
}

void ASoundManager::Play3DSound(int Index,UWorld* world,FVector vector)
{
	if (Index < 0 || Index >= SoundList.Num()) return;//リストに存在しないインデックスなら再生しない
	if (SoundList.GetData()[Index] == nullptr) return;//サウンドが設定されていないなら再生しない

	UGameplayStatics::PlaySoundAtLocation(world, SoundList.GetData()[Index], vector);
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
	//SoundManagerInstance->PlaySound(Index);
}

void ASoundManager::SafePlay3DSound(int Index,UWorld* world, FVector vector)
{
	if (!SoundManagerInstance) return;
	SoundManagerInstance->Play3DSound(Index,world,vector);
}

void ASoundManager::SafeStopSound(int Index)
{
	if (!SoundManagerInstance) return;
	SoundManagerInstance->StopSound(Index);
}
