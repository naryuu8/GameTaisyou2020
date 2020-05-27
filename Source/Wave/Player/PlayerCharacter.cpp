// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.
#include "PlayerCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "../WaterSurface/WaterSurface.h"
#include "../WaterSurface/Raft.h"
#include "Kismet/GameplayStatics.h"
#include "../Object/GameController.h"
#include "../Camera/GameCameraActor.h"
#include "../InputManager.h"
#include "../UI/PauseUI.h"
#include "../UI/HammerCountBarUI.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnimInstance.h"
#include "Niagara/Public/NiagaraFunctionLibrary.h"
#include "../GlobalGameInstance.h"
//////////////////////////////////////////////////////////////////////////
// APlayerCharacter

APlayerCharacter::APlayerCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;
	//ポーズ中でもTickが来るようにする
	SetTickableWhenPaused(true);
}

//void APlayerCharacter::BeginPlay()
//{
//
//
//}

void APlayerCharacter::BeginPlay_C()
{
	//現在のBegibPlayはモデルの都合上こちらで書けないので関数で呼ぶ
	IsAttackHold = false;
	IsPlayAttackAnime = false;
	HammerPower = 0.0f;
	HammerHP = MaxHammerHP;
	if (!AnimInst)
	{
		AnimInst = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());
		// ハンマーで叩いた時に呼ばれる関数を登録
		AnimInst->AttackEndCallBack.BindUObject(this, &APlayerCharacter::HummerAttackEnd);
	}
	CreateHammerCountBarUI();

	PrevPos = FVector::ZeroVector;

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWaterSurface::StaticClass(), FoundActors);
	for (auto Actor : FoundActors)
	{
		AWaterSurface* water = Cast<AWaterSurface>(Actor);
		if (water)
		{
			Water = water;
		}
	}

	IsRaftRiding = false;

	IsRide = true;
}

void APlayerCharacter::Tick(float DeltaTime)
{
	PauseInput();
	if (UGameplayStatics::IsGamePaused(GetWorld()))
	{//ポーズ中はポーズの入力しか受け付けない
		return;
	}
	//アタックアニメが再生中か確認
	IsPlayAttackAnime = AnimInst->GetIsAttackAnime();

	const AInputManager * inputManager = AInputManager::GetInstance();
	if (inputManager)
	{
		const InputState * input = inputManager->GetState();
		if (input->Attack.IsPress) TriggerHammerAttack();
		else if (input->Attack.IsRelease) ReleaseHammerAttack();

		FVector movedPos = GetActorLocation();
		if (input->Select.IsPress && IsRide)
		{
			IsRide = false;

			if (!IsRaftRiding)
			{
				UE_LOG(LogTemp, Log, TEXT("Charenge Ride"));
				TArray<AActor*> FoundActors;
				UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARaft::StaticClass(), FoundActors);
				for (auto Actor : FoundActors)
				{
					ARaft* raft = Cast<ARaft>(Actor);
					if (!raft) continue;
					UE_LOG(LogTemp, Log, TEXT("Found Raft"));
					if (!raft->IsRide(movedPos)) continue;

					UE_LOG(LogTemp, Log, TEXT("Ride"));
					CurrentRaft = raft;
					IsRaftRiding = true;
					FVector RidePos = CurrentRaft->GetActorLocation();
					RidePos.Z = CurrentRaft->GetRiderZ();
					movedPos = RidePos;
					PrevPos = RidePos;
				}
			}
			else
			{
				FVector GetOffPos = CurrentRaft->GetGetOffPos();
				if (GetOffPos != FVector::ZeroVector)
				{
					UE_LOG(LogTemp, Log, TEXT("Get Off"));
					GetOffPos.Z = CurrentRaft->GetRiderZ();
					SetActorLocation(GetOffPos);
					movedPos = GetOffPos;
					IsRaftRiding = false;
				}
			}
		}

		if (input->Select.IsRelease)
		{
			IsRide = true;
		}

		if (IsRaftRiding)
		{
			movedPos = CurrentRaft->GetMoveVec() + movedPos;
			SetActorLocation(movedPos);
			if (!CurrentRaft->IsOnRaft(GetActorLocation()))
			{
				FVector moveVec = movedPos - PrevPos;
				SetActorLocation(CurrentRaft->AdjustMoveOnRaft(PrevPos, moveVec));
			}
			else
			{
				MoveForward(input->LeftStick.Vertical);
				MoveRight(input->LeftStick.Horizontal);
			}
		}
		else if (!Water->IsLand(movedPos))
		{
			if (PrevPos != FVector::ZeroVector)
			{
				FVector moveVec = movedPos - PrevPos;
				moveVec.Z = 0;
				SetActorLocation(Water->AdjustMoveInLand(PrevPos, moveVec, 100.0f));
			}
		}
		else
		{
			MoveForward(input->LeftStick.Vertical);
			MoveRight(input->LeftStick.Horizontal);
		}
		PrevPos = movedPos;
	}

	if (IsAttackHold)
	{//ハンマーを溜めていたら力を足す
		HammerPower += ChargePower;
		MinusHammerGauge(HammerPower);
	}

}

void APlayerCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
}

void APlayerCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	Jump();
}

void APlayerCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	StopJumping();
}

void APlayerCharacter::MoveForward(float Value)
{
	if (IsAttackHold)return;
	if (IsPlayAttackAnime)return;

	if ((FollowCamera != NULL) && (Value != 0.0f))
	{
		FVector Direction = FollowCamera->GetActorForwardVector();
		if (FMath::Abs(Direction.Z) > 0.9f){ Direction = FollowCamera->GetActorUpVector(); } // カメラが真上にある時にも対応
		Direction.Z = 0.0f; Direction.Normalize();
		AddMovementInput(Direction, Value);
	}
}

void APlayerCharacter::MoveRight(float Value)
{
	if (IsAttackHold)return;
	if (IsPlayAttackAnime)return;

	if ((FollowCamera != NULL) && (Value != 0.0f))
	{
		FVector Direction = FollowCamera->GetActorRightVector();
		Direction.Z = 0.0f; Direction.Normalize();
		AddMovementInput(Direction, Value);
	}
}

void APlayerCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void APlayerCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void APlayerCharacter::TriggerHammerAttack(void)
{
	if (IsPlayAttackAnime)return;
	AnimInst = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());
	AnimInst->HummerChergeEvent();
	IsAttackHold = true;
	HammerCountBarUI->FirstEvent();
}

void APlayerCharacter::ReleaseHammerAttack(void)
{
	if (!IsAttackHold) return;
	AnimInst = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());
	AnimInst->HummerAttackEvent();
	IsAttackHold = false;
	HammerCountBarUI->ReflectionGauge();
}

void APlayerCharacter::HummerAttackEnd()
{
	if (AttackEffect && AttackEffect->IsValid() && HummerTipPoint)
	{
		// ハンマーの先端を取得
		FVector AttackPoint = HummerTipPoint->GetComponentLocation();
		// 波を起こす
		WaterAttack(AttackPoint, HammerPower);
		// エフェクトを生成
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, AttackEffect, AttackPoint, GetActorRotation(), FVector::OneVector * AttackEffectScale, true);
		// カメラシェイク
		if(FollowCamera) FollowCamera->EventCameraShake(HammerPower);
	}
	HammerPower = 0.0f;
}

void APlayerCharacter::MinusHammerGauge(const float Power)
{
	if (!HammerCountBarUI)return;
	HammerHP -= ChargePower;
	if (HammerHP < 0.0f)
	{
		HammerHP = 0.0f;
	}
	HammerCountBarUI->UpdateGauge(HammerHP);
}

void APlayerCharacter::PauseInput()
{
	const AInputManager * inputManager = AInputManager::GetInstance();
	if (!inputManager)return;
	const InputState * input = inputManager->GetState();
	if (input->Pause.IsPress)
	{//ポーズ中でなければポーズ画面を開き、ポーズ中だったらポーズ画面を閉じる
		if (!UGameplayStatics::IsGamePaused(GetWorld()))
		{
			if (PauseUIClass != nullptr)
			{//初めてポーズ画面を開くときはUIを生成する
				if (!PauseUI)
				{
					PauseUI = CreateWidget<UPauseUI>(GetWorld(), PauseUIClass);
					PauseUI->AddToViewport();
				}
				else if (PauseUI)
				{
					if (PauseUI->GetIsPlayAnimation())return;
					PauseUI->AddToViewport();
				}
				//生成してもnullptrだったらエラー文表示
				if(PauseUI == nullptr)
				{
					UE_LOG(LogTemp, Error, TEXT("PauseUI : %s"), L"Widget cannot create");
				}
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("PauseUI : %s"), L"PauseUIClass is nullptr");
			}
		}
		else if (UGameplayStatics::IsGamePaused(GetWorld()))
		{
			if (!PauseUI)return;
			if (PauseUI->GetIsPlayAnimation())return;
			PauseUI->EndPlayAnimation();
		}
	}
	if (!UGameplayStatics::IsGamePaused(GetWorld()))return;
	if (!PauseUI)return;
	if (input->Up.IsPress)
	{
		PauseUI->BackSelectState();
	}
	if (input->Down.IsPress)
	{
		PauseUI->NextSelectState();
	}
	if (input->Select.IsPress)
	{
		PauseUI->SelectStateAction();
	}
}

void APlayerCharacter::WaterAttack(FVector Point, float Power)
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWaterSurface::StaticClass(), FoundActors);
	
	for (auto Actor : FoundActors)
	{
		AWaterSurface* water = Cast<AWaterSurface>(Actor);
		if (water)
		{
			water->AddPower(Point, Power * 100.0f);
		}
	}
}

void APlayerCharacter::CreateHammerCountBarUI()
{
	if (HammerCountBarUI)return;
	//BarUI生成
	if (HammerCountBarUIClass != nullptr)
	{
		HammerCountBarUI = CreateWidget<UHammerCountBarUI>(GetWorld(), HammerCountBarUIClass);
		HammerCountBarUI->AddToViewport();
		HammerCountBarUI->SetMaxHammerHP(MaxHammerHP);
		//生成してもnullptrだったらエラー文表示
		if (HammerCountBarUI == nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("PauseUI : %s"), L"Widget cannot create");
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("PauseUI : %s"), L"HammerCountBarUIClass is nullptr");
	}

}

void APlayerCharacter::SetNormaPercent(const float percent)
{
	if (HammerCountBarUI)
	{
		HammerCountBarUI->SetNormaPercent(percent);
	}
	else
	{
		CreateHammerCountBarUI();
		HammerCountBarUI->SetNormaPercent(percent);
	}
}

