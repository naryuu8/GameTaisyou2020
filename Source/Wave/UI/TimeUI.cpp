// Fill out your copyright notice in the Description page of Project Settings.


#include "TimeUI.h"

void UTimeUI::SetOneFrameAngle(const int time_limit)
{
	LimitTime = time_limit;
	//1秒で回転する値を求める
	OneLimitAngle = 360.0f / time_limit;
	//1フレームで回転する値を求める
	OneFrameAngle = OneLimitAngle / 60.0f;
}

void UTimeUI::SetCountDownAngle(const int countdown)
{
	//まず1秒毎の回転値を求める
	CountDownAngle = OneFrameAngle * 60.0f;
	//1秒の回転値とカウントダウン時間をかけて回転値を求める
	CountDownAngle = CountDownAngle * countdown;
	//360を引いて表示する回転値を求める
	CountDownAngle = 360.0f- CountDownAngle;
}

float UTimeUI::SetNormaAngle(const int norma_time)
{
	//まず1秒毎の回転値を求める
	NormaAngle = OneFrameAngle * 60.0f;
	//1秒の回転値とカウントダウン時間をかけて回転値を求める
	NormaAngle = NormaAngle * norma_time;
	//360を引いて表示する回転値を求める
	NormaAngle = 360.0f - NormaAngle;
	return NormaAngle;
}

float UTimeUI::SetNormaAngleTitle(const int norma_time)
{
	//まず1秒毎の回転値を求める
	NormaAngle = OneFrameAngle * 60.0f;
	//1秒の回転値とカウントダウン時間をかけて回転値を求める
	NormaAngle = NormaAngle * norma_time;
	return NormaAngle;
}