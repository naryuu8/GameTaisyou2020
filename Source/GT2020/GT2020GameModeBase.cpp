// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.


#include "GT2020GameModeBase.h"
#include "PlayerCharacter.h"
#include "UObject/ConstructorHelpers.h"

AGT2020GameModeBase::AGT2020GameModeBase()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Main/Player/BP_PlayerCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}