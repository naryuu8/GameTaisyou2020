// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveData.h"

void USaveData::InSaveData(const int number, const FSaveDataStruct savedata)
{
	Save.EmplaceAt(number, savedata);
	//Save.Insert_GetRef(savedata,number);
}

FSaveDataStruct USaveData::GetSaveData(const int number) const
{
	//配列の要素外アクセスだったら空の値を返す
	if (number >= Save.Max())
	{
		return FSaveDataStruct();
	}
	return Save[number];
}
