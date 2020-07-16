// Fill out your copyright notice in the Description page of Project Settings.

#include "Title_BattleCamera.h"
#include "TitleCamera.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "../Object/TitleBattleMemo.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"
#include "../GlobalGameInstance.h"

// Sets default values
ATitle_BattleCamera::ATitle_BattleCamera()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// カメラブームを生成
// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

}

// Called when the game starts or when spawned
void ATitle_BattleCamera::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ATitle_BattleCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	GetTitleBattleMemo();
	//戻るボタンを押したらカメラを引く
	if (IsBack)
	{
		CameraBoom->TargetArmLength += 0.5f;
	}
}

void ATitle_BattleCamera::SelectInput()
{
	if (TitleBattleMemo)
	{
		TitleBattleMemo->FadeStart();
		IsSelectMap = true;
	}
}

void ATitle_BattleCamera::SetCenterBattleMemo()
{
	if (TitleBattleMemo)
	{
		this->SetActorLocation(TitleBattleMemo->GetActorLocation());
	}
}

void ATitle_BattleCamera::GetTitleBattleMemo()
{
	if (TitleBattleMemo)return;
	TitleBattleMemo = Cast<ATitleBattleMemo>(UGameplayStatics::GetActorOfClass(GetWorld(), ATitleBattleMemo::StaticClass()));
}