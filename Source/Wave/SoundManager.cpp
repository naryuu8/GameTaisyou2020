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
	AudioComponent.Add(NewAudioComponent);
}

void ASoundManager::PlaySound(TCHAR* fileName)
{
	for (int i = 0; i < AudioComponent.Num(); i++) {
		if (!AudioComponent[i]->IsPlaying()) {
			AudioComponent[i]->Sound = LoadObject<USoundBase>(NULL,fileName, NULL, LOAD_None, NULL);
			AudioComponent[i]->Play();			
		}
		else {
			auto NewAudioComponent = NewObject<UAudioComponent>(this);
			NewAudioComponent->Sound = LoadObject<USoundBase>(NULL, fileName, NULL, LOAD_None, NULL);
			NewAudioComponent->Play();
			AudioComponent.Add(NewAudioComponent);
		}
	}
}

ASoundManager* ASoundManager::GetInstance()
{
	return SoundManagerInstance;
}

//引数はTCHARなので _T()で囲む　ex)_T("SoundCue'/Game/Main/Sound/hammer_sougen_Cue.hammer_sougen_Cue'")
void ASoundManager::SafePlaySound(TCHAR* fileName)
{
	if (!SoundManagerInstance) return;
	SoundManagerInstance->PlaySound(fileName);
}

UAudioComponent* ASoundManager::CreateAudioComponent(TCHAR* fileName) {
	auto NewAudioComponent = NewObject<UAudioComponent>();
	NewAudioComponent->Sound = LoadObject<USoundBase>(NULL, fileName, NULL, LOAD_None, NULL);
	return NewAudioComponent;
}