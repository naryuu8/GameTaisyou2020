// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TitleMemo.generated.h"
//タイトル用ステージ選択の伝票クラス
class UTitleMemoUI;
class UUserWidget;
class UWidgetComponent;
class UFadeUI;
UCLASS()
class WAVE_API ATitleMemo : public AActor
{
	GENERATED_BODY()
public:	
	// Sets default values for this actor's properties
	ATitleMemo();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Instanced")
		int StageNumber;
	UPROPERTY(EditAnywhere)
		TSubclassOf<UTitleMemoUI> UIClass;
	UPROPERTY(EditAnywhere)
		TSubclassOf<UFadeUI> FadeUIClass;
	UPROPERTY(BlueprintReadWrite, Category = "C++Class")
		UTitleMemoUI* TitleMemoUI;
	UPROPERTY(BlueprintReadWrite, Category = "C++Class")
		FString StageLevelPath;
	UFUNCTION(BlueprintCallable, Category = "C++Library")
		void CreateUI();
	UFUNCTION(BlueprintCallable, Category = "C++Library")
		UTexture2D* GetTexture2D(const FName AssetPath);
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void FadeStart();
	int GetStageNumber() { return StageNumber; }
	UFUNCTION(BlueprintCallable, Category = "C++Library")
	FORCEINLINE UUserWidget* GetUserWidgetObject2(UWidgetComponent* widget)const;
};
