// Fill out your copyright notice in the Description page of Project Settings.


#include "GameCamera.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "../InputManager.h"
#include "../MyFunc.h"

// Sets default values
AGameCamera::AGameCamera()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//コンポーネントを作成
	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USceneComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(this->RootComponent);

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom);
	FollowCamera->bUsePawnControlRotation = false;

	FollowDistance = 300.0f;
	VerticalMin = 0.0f;
	VerticalMax = 150.0f;
}

// Called when the game starts or when spawned
void AGameCamera::BeginPlay()
{
	Super::BeginPlay();
	APlayerController *playerControtller = UGameplayStatics::GetPlayerController(this, 0);
	if (playerControtller)
	{
		playerControtller->SetViewTarget(this);
	}
}

// Called every frame
void AGameCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateRotation();
}

void AGameCamera::UpdateRotation()
{
	FRotator NewRotation = GetActorRotation();

	const InputState * input = AInputManager::GetInstance()->GetState();
	if (!input) return;

	FQuat AddHorizontal, AddVertical;

	// それぞれの軸の回転量を計算
	AddHorizontal = MyFunc::SetAxisAngle(FVector::UpVector, -input->RightStick.Horizontal * 0.01f * RotationSpeed);
	AddVertical = MyFunc::SetAxisAngle(GetActorRightVector(), input->RightStick.Vertical * 0.01f * RotationSpeed);

	// 回転を加算する
	FQuat NewQuat = NewRotation.Quaternion();
	NewQuat = AddHorizontal * NewQuat;
	NewQuat = AddVertical * NewQuat;

	// 極位方向の角度制限
	FVector ForwardVec = NewQuat.GetForwardVector();
	FVector StartForwardVec = ForwardVec;
	StartForwardVec.Z = 0.0f; StartForwardVec.Normalize();

	float DotF = FVector::DotProduct(StartForwardVec, ForwardVec);	// 角度算出
	bool Up_OR_Down = (FVector::DotProduct(FVector::UpVector, ForwardVec) < 0);	// 上か下か

	// 角度算出してオーバーした分だけ戻す
	float DegreeAngle = FMath::RadiansToDegrees(FMath::Acos(DotF));
	bool IsOver = false;
	if (Up_OR_Down)
	{
		DegreeAngle = VerticalMax - DegreeAngle;
		if (DegreeAngle < 0.0f) IsOver = true;
	}
	else
	{
		DegreeAngle = VerticalMin + DegreeAngle;
		if (DegreeAngle > 0.0f) IsOver = true;
	}

	// オーバーした時のみ実行
	if (IsOver)
	{
		AddVertical = MyFunc::SetAxisAngle(GetActorRightVector(), FMath::DegreesToRadians(DegreeAngle));
		NewQuat = AddVertical * NewQuat;
	}

	// 最終的な回転を代入
	SetActorRotation(NewQuat);

	// 子のトランスフォームを更新
	RootComponent->UpdateChildTransforms();

	// CameraBoomのローカル空間で距離を設定
	FollowCamera->SetRelativeLocation(FVector(-FollowDistance, 0.0f, 0.0f));
}

