// Fill out your copyright notice in the Description page of Project Settings.


#include "TitleMemo.h"
#include "../UI/TitleMemoUI.h"
#include "../MyFunc.h"
#include "Components/WidgetComponent.h"
#include "../UI/FadeUI.h"
#include "../GlobalGameInstance.h"
// Sets default values
ATitleMemo::ATitleMemo()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;	

}

void ATitleMemo::CreateUI()
{
	if (!TitleMemoUI)
	{
		TitleMemoUI = CreateWidget<UTitleMemoUI>(GetWorld(), UIClass);
	}
}

UTexture2D* ATitleMemo::GetTexture2D(const FName AssetPath)
{
	UTexture2D* a = MyFunc::LoadTexture2DFromAssetPath(AssetPath);
	return a;
}

UUserWidget * ATitleMemo::GetUserWidgetObject2(UWidgetComponent* widget) const
{
	return widget->GetUserWidgetObject();
}

void ATitleMemo::FadeStart()
{
	UFadeUI* fade = CreateWidget<UFadeUI>(GetWorld(), FadeUIClass);
	if (fade)
	{
		if (fade->GetFadeIsEnable())return;
		UGlobalGameInstance* instance = UGlobalGameInstance::GetInstance();
		if (instance)
		{//ステージ番号をセット
			instance->SetStageNumber(MyStageNumber);
		}
		fade->AddToViewport();
		fade->SetFadeLevel(FColor::Black, 1.0f, FName(*StageLevelPath), true);
	}
}