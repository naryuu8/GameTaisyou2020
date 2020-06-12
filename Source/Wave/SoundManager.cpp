#include "SoundManager.h"

//インスタンスへのアドレス
static ASoundManager* SoundManagerInstance = nullptr;

static TCHAR* FileNames[] =
{ 
	TEXT("SoundCue'/Game/Main/Sound/BGM/title_Cue.title_Cue'"),
	TEXT("SoundCue'/Game/Main/Sound/BGM/stage_Cue.stage_Cue'"),
	TEXT("SoundWave'/Game/Main/Sound/BGM/stage_select_BGM.stage_select_BGM'"),
	TEXT("SoundCue'/Game/Main/Sound/BGM/game_clear_Cue.game_clear_Cue'"),
	TEXT("SoundWave'/Game/Main/Sound/SE/walk_wood/walk_wood_1.walk_wood_1'"),
	TEXT("SoundWave'/Game/Main/Sound/SE/system_kirikae_1.system_kirikae_1'"),
	TEXT("SoundWave'/Game/Main/Sound/SE/walk_sougen/walk_sougen_1.walk_sougen_1'"),
	TEXT("SoundWave'/Game/Main/Sound/SE/hammer_small.hammer_small'"),
	TEXT("SoundWave'/Game/Main/Sound/SE/hammer_medium.hammer_medium'"),
	TEXT("SoundWave'/Game/Main/Sound/SE/hammer_big.hammer_big'"),
	TEXT("SoundWave'/Game/Main/Sound/SE/hammerAttack_small_water.hammerAttack_small_water'"),
	TEXT("SoundWave'/Game/Main/Sound/SE/hammerAttack_midium_water.hammerAttack_midium_water'"),
	TEXT("SoundWave'/Game/Main/Sound/SE/hammerAttack_big_water.hammerAttack_big_water'"),
	TEXT("SoundWave'/Game/Main/Sound/SE/house_in.house_in'"),
	TEXT("SoundWave'/Game/Main/Sound/SE/explosion.explosion'"),
	TEXT("SoundWave'/Game/Main/Sound/SE/fall/fall_1.fall_1'"),
	TEXT("SoundWave'/Game/Main/Sound/SE/player_fall.player_fall'"),
	TEXT("SoundWave'/Game/Main/Sound/SE/explosion.explosionik"),
	TEXT("SoundWave'/Game/Main/Sound/SE/ikada_kisimi_1.ikada_kisimi_1'"),
	TEXT("SoundWave'/Game/Main/Sound/SE/ikada_nami_1.ikada_nami_1'"),
	TEXT("SoundWave'/Game/Main/Sound/SE/Result_drop.result_drop'"),
	TEXT("SoundWave'/Game/Main/Sound/SE/Result_number.result_number'"),
	TEXT("SoundWave'/Game/Main/Sound/SE/result_hanko.result_hanko'"),
	TEXT("SoundWave'/Game/Main/Sound/SE/kirikae/kirikae_1.kirikae_1' "),
	TEXT("SoundWave'/Game/Main/Sound/SE/kettei/kettei_2.kettei_2'"),
	TEXT("SoundWave'/Game/Main/Sound/hammer_charge.hammer_charge'"),
	TEXT("SoundWave'/Game/Main/Sound/SE/over_Quota.over_Quota'"),
	TEXT("SoundWave'/Game/Main/Sound/SE/syukuhuku.syukuhuku'"),
	TEXT("SoundWave'/Game/Main/Sound/SE/time_1.time_1'")
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
	if (SoundManagerInstance == nullptr) return nullptr;
	auto NewAudioComponent = NewObject<UAudioComponent>(SoundManagerInstance);
	TCHAR* fileName = FileNames[(int)type];
	NewAudioComponent->Sound = LoadObject<USoundBase>(NULL, fileName, NULL, LOAD_None, NULL);
	return NewAudioComponent;
}