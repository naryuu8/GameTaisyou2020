// Fill out your copyright notice in the Description page of Project Settings.


#include "HammerCountBarUI.h"
#define GAUGE_SPEED (0.03f)//ゲージ徐々に減らすスピード
float UHammerCountBarUI::DownGauge(const float DamageX, const float HpX)
{
	if (!IsDamageDown)
	{//ゲージが初めて下がる時の差分を得る
		NowHp = DamageX - HpX;
		IsDamageDown = true;
	}
	//1フレームで減らす量を計算
	//今は割合だが固定値にすれば一定の速さになる
	float AddBar = NowHp * GAUGE_SPEED;
	return DamageX - AddBar;
}

float UHammerCountBarUI::DownGaugeTime(const float DamageX, const float HpX, const float Speed)
{
	if (!IsDamageDown)
	{//ゲージが初めて下がる時の差分を得る
		NowHp = DamageX - HpX;
		IsDamageDown = true;
	}
	//1フレームで減らす量を計算
	//float AddBar = NowHp * Speed;
	return DamageX + Speed;
}