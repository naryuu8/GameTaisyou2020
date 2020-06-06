// Fill out your copyright notice in the Description page of Project Settings.

#include "GoalComponent.h"
#include "../WaterSurface/FloatActor.h"
#include "GameController.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UGoalComponent::UGoalComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

//	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("GoalCollision"));
}


// Called when the game starts
void UGoalComponent::BeginPlay()
{
	Super::BeginPlay();

	isGoal = false;
	
//	SphereComp->OnComponentBeginOverlap.AddUniqueDynamic(this, &UGoalComponent::OnOverlapBegin);
//	SphereComp->SetupAttachment(this);
}

void UGoalComponent::OnFloatActorCheck(AActor * OtherActor)
{
	AFloatActor* OtherFloat = Cast<AFloatActor>(OtherActor);
	// 荷物以外は判定しない
	if (isExplotion) return; //すでに壊れているから
	if (!OtherFloat) return;
	AGameController* game = Cast<AGameController>(UGameplayStatics::GetActorOfClass(GetWorld(), AGameController::StaticClass()));
	if (!game) return;

	//ゲーム終了条件を満たしていたら当たり判定を行わない
	if (game->GetIsClear() || game->GetIsGameOver())return;

	// 衝突したアクターが爆弾の時爆発してゴール済みでもゴールしていないことにする
	if (OtherFloat->ActorHasTag("Bom"))
	{
		if (isGoal)
			game->MinusGoalCount();

		game->MinusNotExplotionCount();
		isGoal = false;
		isExplotion = true;
		OtherFloat->Destroy();
		//BreakHome();
		return;
	}

	if (isGoal) return;

	if (OtherFloat->ActorHasTag("Nimotu"))
	{
		// 衝突したアクターが荷物ならゴール済みにする
		isGoal = true;
		game->AddGoalCount();
		// 衝突した荷物を削除
		OtherFloat->Destroy();
		// ここでドアが閉まるアニメーション開始
		//PlayAnimationDoorClose();
	}
}

// Called every frame
void UGoalComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

