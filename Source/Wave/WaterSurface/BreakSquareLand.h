// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../WaterSurface/SquareLand.h"
#include "Components/BoxComponent.h"
#include "BreakSquareLand.generated.h"

/**
 * 
 */
UCLASS()
class WAVE_API ABreakSquareLand : public ASquareLand
{
	GENERATED_BODY()

public:
	ABreakSquareLand();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
		USceneComponent* Scene;
	// â◊ï®ÇÃè’ìÀÇîªífÇ∑ÇÈÉRÉäÉWÉáÉì
	UPROPERTY(EditAnywhere, BluePrintReadWrite)
		UBoxComponent* BoxComp;

	//OnComponentBeginOverlap
	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(Category = "C++Event", BlueprintImplementableEvent, BlueprintCallable)
		void BreakAnime();

private:
	void Break();
};
