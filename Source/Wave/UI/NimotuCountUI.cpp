// Fill out your copyright notice in the Description page of Project Settings.


#include "NimotuCountUI.h"
#include "Kismet/KismetTextLibrary.h"

void UNimotuCountUI::SetStageNimotu(const int count)
{
	StageNimotu = UKismetTextLibrary::Conv_IntToText(count);
}

void UNimotuCountUI::SetStageInNimotu(const int count)
{
	InNimotu = UKismetTextLibrary::Conv_IntToText(count);
}

void UNimotuCountUI::SetNormaNimotu(const int count)
{
	NormaNimotu = UKismetTextLibrary::Conv_IntToText(count);
}

void UNimotuCountUI::SetMaxNimotu(const int count)
{
	MaxNimotu = UKismetTextLibrary::Conv_IntToText(count);
}