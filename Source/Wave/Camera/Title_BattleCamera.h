// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Title_BattleCamera.generated.h"
class ATitleBattleMemo;
UCLASS()
class WAVE_API ATitle_BattleCamera : public AActor
{
	GENERATED_BODY()
		/** Camera boom positioning the camera behind the character */
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;
private:
	ATitleBattleMemo* TitleBattleMemo = nullptr;
	bool IsSelectMap = false;
	bool IsBack = false;
	void GetTitleBattleMemo();
public:
	// Sets default values for this actor's properties
	ATitle_BattleCamera();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Instanced")
		float CameraSpeed = 0.1f;
	void SelectInput();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	//カメラ位置を補完せずバトルボードの前に置く
	void SetCenterBattleMemo();
	void SetBack() { IsBack = true; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	FORCEINLINE bool GetIsSelectMap() const { return IsSelectMap; }

};
