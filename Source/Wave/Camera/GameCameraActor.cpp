// Fill out your copyright notice in the Description page of Project Settings.

#include "GameCameraActor.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"
#include "../InputManager.h"
#include "../WaterSurface/WaterSurface.h"
#include "State/GameCameraStateIdle.h"
#include "GameCameraFocusPoint.h"

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
	GameCameraBoom->SetCamera(Camera);
	//AActor * Actor = GetWorld()->SpawnActor<AActor>(FVector::ZeroVector, FRotator::ZeroRotator);
}

// Called when the game starts or when spawned
void AGameCameraActor::BeginPlay()
{
	Super::BeginPlay();
	
	AWaterSurface * Water = Cast<AWaterSurface>(UGameplayStatics::GetActorOfClass(GetWorld(), AWaterSurface::StaticClass()));
	FVector Center = Water->GetCenterPos();
	SetActorLocation(Center);
	// 初めの位置をステージの中心として保持しておく
	FieldCenterPos = Center;
	float Distance = FVector::Dist(Water->GetStartPos(), Center);
	this->FieldDistance = Distance + FieldDistanceOffset;

	// カメラのビューポートをセット
	APlayerController *playerControtller = UGameplayStatics::GetPlayerController(this, 0);
	if (playerControtller)
	{
		playerControtller->SetViewTarget(this);
	}

	ChangeState(new GameCameraStateIdle());
}

void AGameCameraActor::InputChangeType()
{
	auto IM = AInputManager::GetInstance();
	if (!IM) return;

	auto input = IM->GetState();
	if (input->RightStickButton.IsPress)
	{
		int value = (int)Type + 1;
		if (value >= (int)FollowType::Num) value = 0;
		Type = (FollowType)value;
	}
}

// Called every frame
void AGameCameraActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!State) return;

	// 注目するポイントがあるか確かめる
	UGameplayStatics::GetAllActorsOfClass(this, AGameCameraFocusPoint::StaticClass(), FocusPoints);

	State->OnUpdate(this);
}

void AGameCameraActor::ChangeState(GameCameraState * NewState)
{
	if (!NewState) return;
	this->State = NewState;
	this->State->OnStart(this);
}

