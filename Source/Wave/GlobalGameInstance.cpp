// Fill out your copyright notice in the Description page of Project Settings.


#include "GlobalGameInstance.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "GenericPlatform/GenericApplication.h"

UGlobalGameInstance* UGlobalGameInstance::GetInstance()
{
	UGlobalGameInstance* instance = nullptr;

	if (GEngine)
	{
		FWorldContext* context = GEngine->GetWorldContextFromGameViewport(GEngine->GameViewport);
		instance = Cast<UGlobalGameInstance>(context->OwningGameInstance);
	}

	return instance;
}

bool UGlobalGameInstance::GetIsGamePad() const
{
	auto genericApplication = FSlateApplication::Get().GetPlatformApplication();
	if (genericApplication.Get() != nullptr &&
		genericApplication->IsGamepadAttached())
	{
		return true;
	}

	return false;
}

void UGlobalGameInstance::StageSelectChenge()
{
	IsStageSelectMode = true;
	UGameplayStatics::OpenLevel(this, TEXT("TitleMap"), true);
}