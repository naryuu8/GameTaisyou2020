// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
//タイトル用カメラクラス
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TitleCamera.generated.h"
class USpringArmComponent;
class UCameraComponent;
class ATitleMemo;
UCLASS()
class WAVE_API ATitleCamera : public AActor
{
	GENERATED_BODY()
		/** Camera boom positioning the camera behind the character */
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;
private:
	ATitleMemo* TitleMemo;
	int MemoNum;//ステージ選択伝票の総数を格納
	bool IsSelectMap = false;
	bool IsBack = false;
	int MyStageNumber = 0;
	void GetAllTitleMemo();
public:	
	// Sets default values for this actor's properties
	ATitleCamera();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Instanced")
		int DefaultStageNumber = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Instanced")
		float CameraSpeed = 0.1f;
	void SelectInput();
	void RightInput();
	void LeftInput();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	//カメラ位置を補完せず配達紙の前に置く
	void SetCenterTitleMemo();
	void SetBack() { IsBack = true; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	FORCEINLINE bool GetIsSelectMap() const { return IsSelectMap; }
};
