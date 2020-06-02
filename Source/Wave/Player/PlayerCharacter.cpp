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
#include "../MyFunc.h"
//////////////////////////////////////////////////////////////////////////
// APlayerCharacter

APlayerCharacter::APlayerCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Configure character movement
	//GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	//GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	//GetCharacterMovement()->JumpZVelocity = 600.f;
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
	// シーン上のゲームカメラを検索する
	AGameCameraActor* cameraActor;
	cameraActor = Cast<AGameCameraActor>(UGameplayStatics::GetActorOfClass(GetWorld(), AGameCameraActor::StaticClass()));
	if (cameraActor)
	{
		// 互いにプレイヤーとカメラの参照をセット
		FollowCamera = cameraActor;
		cameraActor->SetFollowTarget(this);
	}

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

	Water = Cast<AWaterSurface>(UGameplayStatics::GetActorOfClass(GetWorld(), AWaterSurface::StaticClass()));

	IsRaftRiding = false;

	IsRide = true;
}

void APlayerCharacter::Tick(float DeltaTime)
{
	//アタックアニメが再生中か確認
	IsPlayAttackAnime = AnimInst->GetIsAttackAnime();
	//アタックアニメ中はポーズを開けないようにする
	if (!IsPlayAttackAnime && !IsAttackHold)
	{
		PauseInput();
		if (UGameplayStatics::IsGamePaused(GetWorld()))
		{//ポーズ中はポーズの入力しか受け付けない
			return;
		}
	}

	const AInputManager * inputManager = AInputManager::GetInstance();
	if (inputManager)
	{
		const InputState * input = inputManager->GetState();
		if (input->Attack.IsPress) TriggerHammerAttack();
		else if (input->Attack.IsRelease) ReleaseHammerAttack();

		float MoveSpeed = 10.0f;
		if (AnimInst->GetIsCharge()) MoveSpeed *= 0.5f;

		// 左スティックの倒し具合の割合を算出
		MoveAmount = FMath::Clamp(FMath::Abs(input->LeftStick.Horizontal) + FMath::Abs(input->LeftStick.Vertical), 0.0f, 1.0f);
		
		// カメラから移動する向きを算出
		FVector Direction;
		if (FollowCamera != NULL)
		{
			FVector V_Direction = FollowCamera->GetActorForwardVector();
			if (FMath::Abs(V_Direction.Z) > 0.9f) { V_Direction = FollowCamera->GetActorUpVector(); } // カメラが真上にある時にも対応
			V_Direction.Z = 0.0f; 
			V_Direction.Normalize();

			FVector H_Direction = FollowCamera->GetActorRightVector();
			H_Direction.Z = 0.0f; 
			H_Direction.Normalize();

			Direction = V_Direction * input->LeftStick.Vertical + H_Direction * input->LeftStick.Horizontal;
			Direction.Normalize();
		}

		// 進む方向に向きを変える
		if (MoveAmount > 0.01f)
		{
			float angle = FMath::Atan2(Direction.Y, Direction.X);
			FQuat LookAt = MyFunc::FromAxisAngleToQuaternion(FVector::UpVector, angle);
			SetActorRotation(FQuat::Slerp(GetActorQuat(), LookAt, 0.1f));
		}

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

		// イカダに乗っている時
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
				Move(Direction, MoveAmount * MoveSpeed);
			}
		}
		// 通常時
		else
		{
			float WaterCheckRadius = Radius * 1.2f;
			float dist = WaterCheckRadius * 1.3f;
			FVector CurPos = GetActorLocation();
			FVector moveForce = Direction * MoveAmount * MoveSpeed;
			FVector moveDir = moveForce.GetSafeNormal();
			FVector WaterCheckPos = CurPos + moveDir * dist;

			UKismetSystemLibrary::DrawDebugCylinder(this, WaterCheckPos - FVector(0, 0, 50), WaterCheckPos - FVector(0, 0, 40), WaterCheckRadius, 64, FLinearColor::Blue, 0.0f, 3.0f);
			UKismetSystemLibrary::DrawDebugCylinder(this, CurPos + moveForce - FVector(0, 0, 50), CurPos + moveForce - FVector(0, 0, 40), Radius, 64, FLinearColor::Red, 0.0f, 3.0f);

			FVector NextPos = Water->AdjustMoveInLand(CurPos, moveForce, Radius, WaterCheckPos, WaterCheckRadius);
			
			float MoveValue = 0.0f;
			(NextPos - CurPos).ToDirectionAndLength(Direction, MoveValue);
			// 最終的に決定した移動量を加算
			Move(Direction, MoveValue);
		}
		PrevPos = movedPos;
	}

	if (IsAttackHold)
	{
		// 溜められる最大値以上はいかない
		if (HammerPower < ChargePowerMax)
		{
			//ハンマーを溜めていたら力を足す
			HammerPower += ChargeSpeed;
			HammerPower = FMath::Min(HammerPower, ChargePowerMax);
			MinusHammerGauge(HammerPower);
		}
		
	}

	//カメラにレイを飛ばして当たらなければアウトライン適用
	ACharacter* myCharacter = this;
	FVector Start = this->GetActorLocation();
	FVector End = (FollowCamera->Camera->GetComponentLocation() - Start) * 10000 + Start;

	FHitResult HitData(ForceInit);
	if (Trace(AActor::GetWorld(), myCharacter, Start, End, HitData) && HitData.GetActor()) OutLineDraw();
	else OutLineNotDraw();
}

void APlayerCharacter::Move(const FVector & Direction, float Value)
{
	if (IsPlayAttackAnime)return;
	if (Value < 0.05f) return;

	//FVector NextPos = GetActorLocation() + Direction * Value;
	//SetActorLocation(NextPos);

	AddMovementInput(Direction, Value);
	// 無理やり移動量を調整
	float FallForce = GetMovementComponent()->Velocity.Z;
	GetMovementComponent()->Velocity = Direction * Value * 40.0f;
	GetMovementComponent()->Velocity.Z = FallForce;
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
		WaterAttack(AttackPoint, HammerPower * HammerPowerValue);
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
	HammerHP -= ChargeSpeed;
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
					//ポーズ用のバーを更新するためHPを渡す
					PauseUI->SetMaxHP(MaxHammerHP);
					PauseUI->SetHP(HammerHP);
				}
				else if (PauseUI)
				{
					if (PauseUI->GetIsPlayAnimation())return;
					PauseUI->AddToViewport();
					PauseUI->SetHP(HammerHP);
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

void APlayerCharacter::HammerCountBarParent()
{
	if (HammerCountBarUI)
	{
		HammerCountBarUI->RemoveFromParent();
	}
}