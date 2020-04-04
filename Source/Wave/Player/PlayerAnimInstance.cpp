// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnimInstance.h"
#include "GameFramework/Actor.h"
#include "UObject/ConstructorHelpers.h"

UPlayerAnimInstance::UPlayerAnimInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void UPlayerAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	Speed = 0.0f;
	IsAttackAnime = false;
	
}

void UPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

}

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);
	
}

void UPlayerAnimInstance::NativePostEvaluateAnimation()
{
	Super::NativePostEvaluateAnimation();

}

void UPlayerAnimInstance::NativeUninitializeAnimation()
{
	Super::NativeUninitializeAnimation();

}

void UPlayerAnimInstance::HummerChergeEvent()
{
	//const FName AnimMontageAssetPath(TEXT("AnimMontage'/Game/Main/Player/HummerAttackMontage.HummerAttackMontage'"));
	//auto AnimMontage = Cast<UAnimMontage>(StaticLoadObject(UObject::StaticClass(), nullptr, TEXT("AnimMontage'/Game/Main/Player/HummerAttackMontage.HummerAttackMontage'")));
	//

	//////例外エラーが出るので今は封印
	////const FName AnimMontageAssetPath(TEXT("/Game/Main/Player/HummerAttackMontage"));

	////UAnimMontage* AnimMontage = Cast<UAnimMontage>(StaticLoadObject(UObject::StaticClass(), nullptr, *AnimMontageAssetPath.ToString()));
	//this->Montage_Play(AnimMontage, 0.4f, EMontagePlayReturnType::MontageLength,0.0f,true);
	//this->Montage_JumpToSection("Charge");
	//IsAttackAnime = true;
	//this->Montage_Pause(AnimMontage);
}

void UPlayerAnimInstance::HummerAttackEvent()
{
	//const FName AnimMontageAssetPath(TEXT("/Game/Main/Player/HummerAttackMontage"));
	//UAnimMontage* AnimMontage = Cast<UAnimMontage>(StaticLoadObject(UObject::StaticClass(), nullptr, *AnimMontageAssetPath.ToString()));
	//Montage_Play(AnimMontage,0.4f);
	//Montage_JumpToSection("Attack");
}