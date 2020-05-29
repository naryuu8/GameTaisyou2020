// Fill out your copyright notice in the Description page of Project Settings.

#include "GameCameraComponent.h"
#include "Camera/CameraComponent.h"
#include "../InputManager.h"
#include "../MyFunc.h"

// Sets default values for this component's properties
UGameCameraComponent::UGameCameraComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	bAutoActivate = true;
	bTickInEditor = true;
}

void UGameCameraComponent::BeginPlay()
{
	Super::BeginPlay();

	if (FollowCamera)
	{
		// 初期のカメラ距離で初期化
		FollowDistance = FollowCamera->GetRelativeLocation().Z;
	}
}

void UGameCameraComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdateRotation();

	// CameraBoomのローカル空間で距離を設定
	if (FollowCamera)
	{
		// なめらかに距離を変更
		FollowDistance = FMath::Lerp<float>(FollowDistance, TargetFollowDistance, 0.1f);
		FollowCamera->SetRelativeLocation(FVector(-FollowDistance, 0.0f, 0.0f));
	}
}

void UGameCameraComponent::UpdateRotation()
{
	const AInputManager * inputManager = AInputManager::GetInstance();
	if (!inputManager) return;
	const InputState * input = inputManager->GetState();

	FQuat NewQuat;
	FQuat AddHorizontal, AddVertical;
	// ワールド系の回転を取得
	NewQuat = GetRelativeRotationFromWorld(GetRelativeRotation().Quaternion());

	// それぞれの軸の回転量を計算
	AddVertical = MyFunc::FromAxisAngleToQuaternion(NewQuat.GetRightVector(), input->RightStick.Vertical * 0.01f * RotationSpeed).GetNormalized();
	AddHorizontal = MyFunc::FromAxisAngleToQuaternion(FVector::UpVector, -input->RightStick.Horizontal * 0.01f * RotationSpeed).GetNormalized();
	// 回転を加算する
	NewQuat = AddVertical * NewQuat;	// 極位方向を先に計算
	NewQuat = AddHorizontal * NewQuat;

	// 極位方向の角度制限内かチェック
	if (CheckVerticalLimit(NewQuat, AddVertical))
	{
		NewQuat = AddVertical * NewQuat;
	}
	NewQuat.Normalize();

	// 最終的な回転を代入
	SetWorldRotation(NewQuat);

	// 子のトランスフォームを更新
	UpdateChildTransforms();
}

bool UGameCameraComponent::CheckVerticalLimit(const FQuat & CheckQuat, FQuat & OutOverQuat)
{
	FVector ForwardVec = CheckQuat.GetForwardVector();
	FVector StartForwardVec = ForwardVec;
	StartForwardVec.Z = 0.0f; StartForwardVec.Normalize();
	if (CheckQuat.GetUpVector().Z < 0.0f) StartForwardVec = -StartForwardVec;	// カメラが逆さになっていたらベクトル反転

	float DotF = FVector::DotProduct(StartForwardVec, ForwardVec);
	bool Up_OR_Down = (FVector::DotProduct(FVector::UpVector, ForwardVec) < 0);	// 上か下か

	float DegreeAngle = FMath::RadiansToDegrees(FMath::Acos(DotF));	// 角度算出
	if (Up_OR_Down)
	{
		DegreeAngle = VerticalMax - DegreeAngle;
		if (DegreeAngle > 0.0f) return false;
	}
	else
	{
		DegreeAngle = VerticalMin + DegreeAngle;
		if (DegreeAngle <= 0.0f) return false;
	}

	// 角度算出してオーバーした分を代入
	OutOverQuat = MyFunc::FromAxisAngleToQuaternion(CheckQuat.GetRightVector(), FMath::DegreesToRadians(DegreeAngle)).GetNormalized();
	return true;
}

