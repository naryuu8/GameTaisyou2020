// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TitlePlayer.generated.h"

UCLASS()
class WAVE_API ATitlePlayer : public ACharacter
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere)
		AActor* TargetPoint = nullptr;
	FVector FirstPosition;
public:
	// Sets default values for this character's properties
	ATitlePlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	UFUNCTION(BlueprintCallable, Category = "C++Library")
		void BeginPlay_C();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void TargetRotation();//ターゲットの方に向く
	void TargetMove();//ターゲットの方に移動する
};
