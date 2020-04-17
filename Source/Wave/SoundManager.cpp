#include "SoundManager.h"

//インスタンスへのアドレス
static ASoundManager* SoundManagerInstance = nullptr;

// Sets default values
ASoundManager::ASoundManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("ModifySkeletalMeshComponent"));
	SoundManagerInstance = this;
}

void ASoundManager::PlaySound(int Index)
{
	if (Index < 0 || Index >= SoundList.Num()) return;//リストに存在しないインデックスなら再生しない
	if (SoundList.GetData()[Index] == nullptr) return;//サウンドが設定されていないなら再生しない
	AudioComponent->Sound = SoundList.GetData()[Index];//サウンド設定
	AudioComponent->Play();//サウンドを再生する
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