// Fill out your copyright notice in the Description page of Project Settings.


#include "Goal.h"
#include "UObject/ConstructorHelpers.h"
#include "UObject/UObjectGlobals.h"
// Sets default values
AGoal::AGoal()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Scene = CreateDefaultSubobject <USceneComponent>(TEXT("Scene"));
	RootComponent = Scene;
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("GoalCollision"));
	SphereComp->SetupAttachment(RootComponent);
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &AGoal::OnOverlapBegin);
}

// Called when the game starts or when spawned
void AGoal::BeginPlay()
{
	Super::BeginPlay();
	isGoal = false;
	CreateHammerCountUI();
}

void AGoal::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	isGoal = true;
}

// Called every frame
void AGoal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGoal::CreateHammerCountUI()
{
	//ハンマーカウントが0なら生成しない
	if (HammerCount == 0)return;
	if (UIClass != nullptr)
	{
		HammerCountUI = CreateWidget<UHammerCountUI>(GetWorld(), UIClass);
		if (HammerCountUI != nullptr)
		{
			HammerCountUI->AddToViewport();
			HammerCountUI->SetHammerCount(HammerCount);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("TitleManager : %s"), L"Widget cannot create");
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("TitleManager : %s"), L"UIClass is nullptr");
	}
}

UUserWidget* AGoal::WidgetCreate(TSubclassOf<UUserWidget> UserWidgetClass)
{
	UUserWidget* widget = CreateWidget(GetWorld(), UserWidgetClass);
	widget->AddToViewport();
	return widget;
}

void AGoal::MinusHammerCount()
{
	if (HammerCount == 0)return;
	if (!HammerCountUI)return;
	HammerCountUI->MinusHammerCount();
	HammerCountUI->MinusCountAnimation();
}
