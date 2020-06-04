// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../WaterSurface/LandPoint.h"
#include "Components/SphereComponent.h"
#include "BreakLand.generated.h"

/**
 * 
 */
UCLASS()
class WAVE_API ABreakLand : public ALandPoint
{
	GENERATED_BODY()


public:
	// â◊ï®ÇÃè’ìÀÇîªífÇ∑ÇÈÉRÉäÉWÉáÉì
	UPROPERTY(EditAnywhere, BluePrintReadWrite)
		USphereComponent* SphereComp;

	//OnComponentBeginOverlap
	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	UFUNCTION(Category = "C++Event", BlueprintImplementableEvent, BlueprintCallable)
		void BreakAnime();

private:
	void Break();		
};
