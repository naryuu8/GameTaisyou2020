// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../WaterSurface/FloatActor.h"
#include "Components/SphereComponent.h"
#include "BreakFloatActor.generated.h"

/**
 * 
 */
UCLASS()
class WAVE_API ABreakFloatActor : public AFloatActor
{
	GENERATED_BODY()

public:
	ABreakFloatActor();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
		USceneComponent* Scene;
	// â◊ï®ÇÃè’ìÀÇîªífÇ∑ÇÈÉRÉäÉWÉáÉì
	UPROPERTY(EditAnywhere, BluePrintReadWrite)
		USphereComponent* SphereComp;

	//OnComponentBeginOverlap
	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(Category = "C++Event", BlueprintImplementableEvent, BlueprintCallable)
		void BreakAnime();
private:

	bool IsExplotion;
};
