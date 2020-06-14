// Fill out your copyright notice in the Description page of Project Settings.


#include "BackGroundUI.h"

#include "../SoundManager.h"

bool UBackGroundUI::GetIsAllStamp() const
{
	if (ResultSaveData.IsStamp[0] == true && ResultSaveData.IsStamp[1] == true && ResultSaveData.IsStamp[2] == true)
	{
		return true;
	}
	return false;
}

bool UBackGroundUI::GetIsStamp(const int index) const
{
	return ResultSaveData.IsStamp[index];
}