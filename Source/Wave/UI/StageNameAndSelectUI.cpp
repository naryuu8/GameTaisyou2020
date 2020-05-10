// Fill out your copyright notice in the Description page of Project Settings.


#include "StageNameAndSelectUI.h"
#include "../MyFunc.h"

UTexture2D* UStageNameAndSelectUI::GetTexture2D(const FName AssetPath)
{
	return MyFunc::LoadTexture2DFromAssetPath(AssetPath);
}