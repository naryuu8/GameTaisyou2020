// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.
#include "PlayerCharacter.h"
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
#include "EngineGlobals.h"
#include "Components/CapsuleComponent.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"
#include "GameFramework/PlayerController.h"

#define DISPLAY_LOG(fmt, ...) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT(fmt), __VA_ARGS__));
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

	OldAttackFrame = false;
	FreasTime = 0;
}

void APlayerCharacter::BeginPlay_C()
{
	// 自身のカプセルコリジョンの大きさを取得
	UCapsuleComponent * Collision = Cast<UCapsuleComponent>(GetComponentByClass(UCapsuleComponent::StaticClass()));
	if (Collision)
		this->CollisionRadius = Collision->GetScaledCapsuleRadius();

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

	Water = Cast<AWaterSurface>(UGameplayStatics::GetActorOfClass(GetWorld(), AWaterSurface::StaticClass()));

	CurrentRaft = nullptr;
	IsDeth = false;
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
	// 死亡時は何もできない
	if (IsDeth) return;

	FVector CurPos = GetActorLocation();

	// 更新の始めにプレイヤーの下を確認する
	if (CheckGround())
	{
		
	}
	// もし失敗したらプレイヤーは落下した判定にする
	else
	{
		Water->SetCollisionEnabled(false);	// WaterSurfaceのコリジョンをオフに設定

		if (CurPos.Z < -10.0f) // プレイヤーの座標が一定以下に行った時は死亡
		{
			Water->AddPower(FVector(CurPos.X, CurPos.Y, 0.0f), ChargePowerMax);
			IsDeth = true;
			PlayerDeth();
			UCapsuleComponent * Collision = Cast<UCapsuleComponent>(GetComponentByClass(UCapsuleComponent::StaticClass()));
			if (Collision)Collision->SetActive(false);
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
		if (AnimInst->GetIsCharge()) MoveSpeed *= 0.3f;

		// 左スティックの倒し具合の割合を算出
		MoveAmount = FMath::Clamp(FMath::Abs(input->LeftStick.Horizontal) + FMath::Abs(input->LeftStick.Vertical), 0.0f, 1.0f);
		// カメラから移動する向きを算出
		FVector Direction = GetInputDirection(input->LeftStick.Vertical, input->LeftStick.Horizontal);
		// 進む方向に向きを変える
		if (MoveAmount > 0.01f)
			SetLookAt(Direction, 0.15f);

		FVector moveForce = Direction * MoveAmount * MoveSpeed;
		FVector moveDir = moveForce.GetSafeNormal();

		// イカダに乗っている時
		if (CurrentRaft != nullptr)
		{
			FHitResult HitResult(ForceInit);
			
			if (!IsInRaft)
			{
				if (CurrentRaft->IsInRaft(CurPos, CollisionRadius))
				{
					IsInRaft = true;
				}
				// イカダの上にいるがはみ出ている状態
				else
				{
					// 強制的にイカダの上に移動する
					FVector SubDirection = CurrentRaft->GetActorLocation() - CurPos;
					MoveAmount = 1.0f;
					SetLookAt(SubDirection, 0.5f);
					Move(SubDirection.GetSafeNormal2D(), MoveSpeed * 0.5f);
				}
			}
			else if (Water->GetLandPoint(CurPos + Direction * CollisionRadius * 2.0f, CollisionRadius * 0.5f) ||			// 陸を見つけた時
					CheckTraceGround(HitResult, CurPos + Direction * CollisionRadius * 2.0f, CollisionRadius * 0.3f, CurrentRaft))   // 他のイカダを見つけた時
			{
				Move(Direction, MoveSpeed * MoveAmount);
			}
			// イカダの上に完全に乗っている状態
			else
			{
				FVector NextPos = CurrentRaft->AdjustMoveOnRaft(CurPos, moveForce, CollisionRadius);
				float MoveValue = 0.0f;
				(NextPos - CurPos).ToDirectionAndLength(Direction, MoveValue);
				Move(Direction, MoveValue);
			}
		}
		// 通常時
		else
		{
			// イカダを見つけた時
			FHitResult HitResult(ForceInit);
			if (CheckTraceGround(HitResult, CurPos + Direction * CollisionRadius * 2.0f, CollisionRadius * 0.5f))
			{
				Move(Direction, MoveSpeed * MoveAmount);
			}
			// 地上にいる時
			else
			{
				float WaterCheckRadius = CollisionRadius * 1.2f;
				float dist = WaterCheckRadius * 1.3f;
				FVector WaterCheckPos = CurPos + moveDir * dist;

				// デバッグ描画
				UKismetSystemLibrary::DrawDebugCylinder(this, WaterCheckPos - FVector(0, 0, 50), WaterCheckPos - FVector(0, 0, 40), WaterCheckRadius, 64, FLinearColor::Blue, 0.0f, 3.0f);
				UKismetSystemLibrary::DrawDebugCylinder(this, CurPos + moveForce - FVector(0, 0, 50), CurPos + moveForce - FVector(0, 0, 40), CollisionRadius, 64, FLinearColor::Red, 0.0f, 3.0f);

				FVector NextPos = Water->AdjustMoveInLand(CurPos, moveForce, CollisionRadius, WaterCheckPos, WaterCheckRadius);
				float MoveValue = 0.0f;
				(NextPos - CurPos).ToDirectionAndLength(Direction, MoveValue);
				// 最終的に決定した移動量を加算
				Move(Direction, MoveValue);
			}
		}
	}

	// チャージ中
	UpdateGaugeHP();
	if (IsAttackHold)
	{
		ChageCreateEmmiter();
		// 溜められる最大値以上はいかない
		if (HammerPower < ChargePowerMax)
		{
			//ハンマーを溜めていたら力を足す
			//体力がほぼ0なら溜めても加算しない
			if (HammerHP > 0.1f)
			{
				HammerPower += ChargeSpeed;
				HammerPower = FMath::Min(HammerPower, ChargePowerMax);
			}
			MinusHammerGauge(HammerPower);
		}
		else
		{//最大溜め時はゲージを点滅させる
			HammerCountBarUI->PlayGaugeAnimation();
		}
		
	}
	else
	{
		ChageDestroyEmmiter();
	}


	
	if (!OldAttackFrame && IsPlayAttackAnime)
	{
		ImpactEmmiterCreate(FreasTime);
		FreasTime = 0;
	}
	OldAttackFrame = IsPlayAttackAnime;
	if (IsAttackHold) FreasTime++;
	

	ChageUpDateEmmiter(CurPos);



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

	AddMovementInput(Direction, Value);
	// 無理やり移動量を調整
	UPawnMovementComponent * Movement = GetMovementComponent();
	float FallForce = Movement->Velocity.Z;
	float MoveScale = Value * 40.0f;
	// 移動できる量が超えていたら調整
	if (Movement->Velocity.SizeSquared2D() > MoveScale)
	{
		Movement->Velocity = Direction * MoveScale;
		Movement->Velocity.Z = FallForce;
	}
}

bool APlayerCharacter::CheckTraceGround(FHitResult & Result, const FVector & Start, float SphereRadius, AActor* Ignore)
{
	FVector End = Start - FVector::UpVector * 10000.0f;

	FCollisionQueryParams TraceParms(FName(TEXT("Player Check Ground")));
	TraceParms.bTraceComplex = false;			// 複雑な衝突を追跡する必要があるかどうか
	TraceParms.bReturnPhysicalMaterial = false; // 結果に物理的な資料を含めるかどうか。
	TraceParms.AddIgnoredActor(this);			// 自分は無視する
	if(Ignore) TraceParms.AddIgnoredActor(Ignore);
	ECollisionChannel CollisionChannel = ECC_Vehicle;	// 乗り物設定したコリジョンだけ判定する
	FCollisionShape SphereCast = FCollisionShape::MakeSphere(SphereRadius); // プレイヤーの大きさの球で判定する
	if (GetWorld()->SweepSingleByObjectType(Result, Start, End, FQuat::Identity, CollisionChannel, SphereCast, TraceParms))
		return true;

	return false;
}

void APlayerCharacter::ResetRaftParam()
{
	CurrentRaft = nullptr;
	IsInRaft = false;
}

void APlayerCharacter::TriggerHammerAttack(void)
{
	if (IsPlayAttackAnime)return;
	//クールタイムがあるときと最大HPとHPが同じなら構えれない
	//if (MaxHammerHP == HammerHP)return;
	if (HammerHP <= 0.0f)return;
	if (CoolTime != 0.0f)return;
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
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
		if (PlayerController)
		{//コントローラー振動　引数1:強さ(0.0-1.0)引数2:時間 残りの引数:コントローラーのどの部分を振動させるか？（全部trueで全体を振動)
			//振動の強さを求める
			float power = ChargeCount / ChargePowerMax;
			PlayerController->PlayDynamicForceFeedback(power, 0.35f, true, true, true, true);
		}	
	}
	//最大溜め状態だったら点滅アニメを停止
	if (HammerPower >= ChargePowerMax)
	{
		HammerCountBarUI->PouseGaugeAnimation();
	}
	HammerPower = 0.0f;
	ChargeCount = 0.0f;
}

void APlayerCharacter::MinusHammerGauge(const float Power)
{
	if (!HammerCountBarUI)return;
	if (ChargeCount < ChargePowerMax)
	{//最大溜め時はクールタイムを加算しない
		//更に現在のHPがMAXHPより下だったら加算
		//if (MaxHammerHP > HammerHP)
		if (HammerHP > 0.0f)
		{
			CoolTime += ChargeSpeed;
			if (CoolTime >= ChargePowerMax)
			{
				CoolTime = ChargePowerMax;
			}
			HammerHP -= ChargeSpeed;
			if (HammerHP >= MaxHammerHP)
			{
				HammerHP = MaxHammerHP;
			}
			ChargeCount += ChargeSpeed;
			if (ChargeCount >= ChargePowerMax)
			{
				ChargeCount = ChargePowerMax;
			}
		}
	}

	HammerCountBarUI->UpdateGauge(HammerHP);
	HammerCountBarUI->UpdateCoolTime(CoolTime);
}

bool APlayerCharacter::CheckGround()
{
	FVector Pos = GetActorLocation();
	if (!Water->IsInField(Pos)) return false;	// フィールド外に出てもfalse

	FHitResult HitData(ForceInit);
	if (CheckTraceGround(HitData, Pos, CollisionRadius * 0.8f))
	{
		ARaft* HitRaft = Cast<ARaft>(HitData.GetActor());
		// イカダに乗った状態で違うイカダに乗った時
		if (HitRaft && HitRaft != CurrentRaft)
			ResetRaftParam();
		// イカダなら登録しておく
		CurrentRaft = HitRaft;

		if(CurrentRaft && !Water->GetLandPoint(Pos))
			Water->SetCollisionEnabled(false);	// 何かの上に乗ってる場合はWaterSurfaceコリジョンをオフにしておく

		//DISPLAY_LOG("HitGroundActor : %s", HitData.GetActor()->GetName().GetCharArray().GetData());
		return true;
	}
	// イカダから降りた時点でイカダで使う変数をリセット
	ResetRaftParam();

	/// 地面の上に存在していなかったらWaterSurfaceコリジョンをオフにしておく
	Water->SetCollisionEnabled(true);

	// 下が陸判定ならその時点でtrue
	if (Water->GetLandPoint(Pos) != nullptr)
		return true;

	return false;
}

FVector APlayerCharacter::GetInputDirection(float VerticalValue, float HolizontalValue)
{
	if (FollowCamera == NULL) return FVector::ZeroVector;

	FVector Result;

	FVector V_Direction = FollowCamera->GetActorForwardVector();
	if (FMath::Abs(V_Direction.Z) > 0.9f) { V_Direction = FollowCamera->GetActorUpVector(); } // カメラが真上にある時にも対応
	V_Direction.Z = 0.0f;
	V_Direction.Normalize();

	FVector H_Direction = FollowCamera->GetActorRightVector();
	H_Direction.Z = 0.0f;
	H_Direction.Normalize();

	Result = V_Direction * VerticalValue + H_Direction * HolizontalValue;
	Result.Normalize();

	return Result;
}

void APlayerCharacter::SetLookAt(FVector Direction, float Speed)
{
	float angle = FMath::Atan2(Direction.Y, Direction.X);
	FQuat LookAt = MyFunc::FromAxisAngleToQuaternion(FVector::UpVector, angle);
	SetActorRotation(FQuat::Slerp(GetActorQuat(), LookAt, Speed));
}

void APlayerCharacter::PauseInput()
{
	const AInputManager * inputManager = AInputManager::GetInstance();
	if (!inputManager)return;
	const InputState * input = inputManager->GetState();
	if (input->Pause.IsPress)
	{//ポーズ中でなければポーズ画面を開き、ポーズ中だったらポーズ画面を閉じる
		AGameController* game = Cast<AGameController>(UGameplayStatics::GetActorOfClass(GetWorld(), AGameController::StaticClass()));
		//ゲーム終了条件を満たしていたらポーズを開けないようにする
		if (game->GetIsClear() || game->GetIsGameOver())return;
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
					if (game)
					{
						game->SetTimeCountPause();
					}
				}
				else if (PauseUI)
				{
				if (PauseUI->GetIsPlayAnimation())return;
				PauseUI->AddToViewport();
				PauseUI->SetHP(HammerHP);
				if (game)
				{
					game->SetTimeCountPause();
				}
				}
				//生成してもnullptrだったらエラー文表示
				if (PauseUI == nullptr)
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
			if (game)
			{
				game->SetTimeCountRePlay();
			}
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
		HammerCountBarUI->SetMaxChargePowerMax(ChargePowerMax);
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

void APlayerCharacter::HammerCountBarParent()
{
	if (HammerCountBarUI)
	{
		HammerCountBarUI->RemoveFromParent();
	}
}

void APlayerCharacter::SetNoTick()
{
	this->SetActorTickEnabled(false);
	MoveAmount = 0.0f;
}

void APlayerCharacter::SetPlayerHiddenInGame()
{
	this->SetActorHiddenInGame(true);
}

void APlayerCharacter::UpdateGaugeHP()
{
	if (IsPlayAttackAnime)return;
	if (IsAttackHold)return;
	if (HammerPower > 0.0f)return;
	CoolTime -= ChargeSpeed * CoolTimeHealSpped;

	if (CoolTime < 0.0f)
	{
		CoolTime = 0.0f;
	}
	if (HammerCountBarUI)
	{
		if (CoolTime <= 0.0f)
		{
			HammerHP += ChargeSpeed * HpHealSpped;
			if (HammerHP > MaxHammerHP)
			{
				HammerHP = MaxHammerHP;
			}
			HammerCountBarUI->UpdateGauge(HammerHP);
		}
		HammerCountBarUI->UpdateCoolTime(CoolTime);
	}
}