// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Goal.generated.h"

UCLASS()
class WAVE_API AGoal : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGoal();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
		USceneComponent* Scene;

	UPROPERTY(EditAnywhere, BluePrintReadWrite)
		USphereComponent* SphereComp;
	UPROPERTY(BlueprintReadWrite, Category = "C++Class")
		bool isGoal;
	//OnComponentBeginOverlap
	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable, Category = "C++Library")
		FORCEINLINE bool GetIsGoal() const { return isGoal; }
private:


};