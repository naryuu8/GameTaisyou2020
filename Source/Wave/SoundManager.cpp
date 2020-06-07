#include "SoundManager.h"

//インスタンスへのアドレス
static ASoundManager* SoundManagerInstance = nullptr;

static TCHAR* FileNames[] =
{
	TEXT("title_Cue"),
	TEXT("walk_wood"),
	TEXT("system_kirikae_1"),
	TEXT("walk_sougen"),
	TEXT("hammer_small"),
	TEXT("hammer_medium"),
	TEXT("hammer_big"),
	TEXT("house_in"),
	TEXT("explosion"),
	TEXT("fall"),
	TEXT("player_fall"),
	TEXT("explosion"),
	TEXT("ikada_kishimi_1"),
	TEXT("ikada_nami_1"),
	TEXT("result_drop"),
	TEXT("result_number"),
	TEXT("kirikae_1 "),
	TEXT("kettei_2")
};

// Sets default values
ASoundManager::ASoundManager()
{
	PrimaryActorTick.bCanEverTick = true;

	SoundManagerInstance = this;
}

void ASoundManager::BeginPlay() {

	auto NewAudioComponent = NewObject<UAudioComponent>(this);
	AudioComponent.Add(NewAudioComponent);
}

void ASoundManager::PlaySound(SOUND_TYPE type)
{
	TCHAR* fileName = FileNames[(int)type];

	for (int i = 0; i < AudioComponent.Num(); i++) {
		if (!AudioComponent[i]->IsPlaying()) {
			AudioComponent[i]->Sound = LoadObject<USoundBase>(NULL,fileName, NULL, LOAD_None, NULL);
			AudioComponent[i]->Play();
			break;
		}
		
		if(AudioComponent.Num() == i + 1){
			auto NewAudioComponent = NewObject<UAudioComponent>(this);
			NewAudioComponent->Sound = LoadObject<USoundBase>(NULL, fileName, NULL, LOAD_None, NULL);
			NewAudioComponent->Play();
			AudioComponent.Add(NewAudioComponent);
			break;
		}
	}
}

ASoundManager* ASoundManager::GetInstance()
{
	return SoundManagerInstance;
}

//引数はTCHARなので _T()で囲む　ex)_T("SoundCue'/Game/Main/Sound/hammer_sougen_Cue.hammer_sougen_Cue'")
void ASoundManager::SafePlaySound(SOUND_TYPE type)
{
	if (!SoundManagerInstance) return;
	SoundManagerInstance->PlaySound(type);
}

UAudioComponent* ASoundManager::CreateAudioComponent(SOUND_TYPE type) {
	auto NewAudioComponent = NewObject<UAudioComponent>();
	TCHAR* fileName = FileNames[(int)type];
	NewAudioComponent->Sound = LoadObject<USoundBase>(NULL, fileName, NULL, LOAD_None, NULL);
	return NewAudioComponent;
}