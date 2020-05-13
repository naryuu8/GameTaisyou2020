// Fill out your copyright notice in the Description page of Project Settings.


#include "GlobalGameInstance.h"
#include "Engine/Engine.h"

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