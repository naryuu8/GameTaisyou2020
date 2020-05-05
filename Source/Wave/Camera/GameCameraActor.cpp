// Fill out your copyright notice in the Description page of Project Settings.

#include "GameCameraActor.h"
#include "Kismet/GameplayStatics.h"
#include "GameCameraComponent.h"
#include "Camera/CameraComponent.h"


// Sets default values
AGameCameraActor::AGameCameraActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// カメラブームを生成
	GameCameraBoom = CreateDefaultSubobject<UGameCameraComponent>(TEXT("GameCameraBoom"));
	GameCameraBoom->SetupAttachment(RootComponent);

	// カメラを生成
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	Camera->SetupAttachment(GameCameraBoom);
	Camera->bUsePawnControlRotation = false;

	// カメラブームのメンバにアタッチ
	GameCameraBoom->FollowCamera = Camera;
}

// Called when the game starts or when spawned
void AGameCameraActor::BeginPlay()
{
	Super::BeginPlay();
	
	// カメラのビューポートをセット
	APlayerController *playerControtller = UGameplayStatics::GetPlayerController(this, 0);
	if (playerControtller)
	{
		playerControtller->SetViewTarget(this);
	}
}

// Called every frame
void AGameCameraActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

