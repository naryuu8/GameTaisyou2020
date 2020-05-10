// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
// ゴールクラス
// オブジェクトが触れたらゴールUIを表示する
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Goal.generated.h"
class UHammerCountUI;
UCLASS()
class WAVE_API AGoal : public AActor
{
	GENERATED_BODY()
private:
	// 表示するUI　エディタで指定する
	UPROPERTY(EditAnywhere)
		TSubclassOf<UHammerCountUI> UIClass;
public:
	// Sets default values for this actor's properties
	AGoal();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	//ハンマーを叩ける回数
	UPROPERTY(EditAnywhere, Category = "Instanced")
		int HammerCount = 0;
	UPROPERTY()
		USceneComponent* Scene;

	UPROPERTY(EditAnywhere, BluePrintReadWrite)
		USphereComponent* SphereComp;
	UPROPERTY(BlueprintReadWrite, Category = "C++Class")
		bool isGoal;
	//OnComponentBeginOverlap
	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable, Category = "C++Library")
		UUserWidget* WidgetCreate(TSubclassOf<UUserWidget> UserWidgetClass);
	UFUNCTION(BlueprintCallable, Category = "C++Library")
		FORCEINLINE bool GetIsGoal() const { return isGoal; }
	void MinusHammerCount();
private:
	UHammerCountUI* HammerCountUI;
	//ハンマーを叩ける回数が1以上ならUIを生成
	void CreateHammerCountUI();
};