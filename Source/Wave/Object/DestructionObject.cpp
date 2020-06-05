// Fill out your copyright notice in the Description page of Project Settings.


#include "DestructionObject.h"
#include "Kismet/GameplayStatics.h"
// Sets default values
ADestructionObject::ADestructionObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	sphere = CreateDefaultSubobject<USphereComponent>(TEXT("KKKK"));
	sphere->InitSphereRadius(100.0f);
	//sphere->SetSimulatePhysics = false;
	sphere->SetCollisionProfileName(TEXT("BlockAll"));
	sphere->OnComponentBeginOverlap.AddUniqueDynamic(this, &ADestructionObject::OnOverlapBegin);
	RootComponent = sphere;


	//Scene = CreateDefaultSubobject <USceneComponent>(TEXT("Scene"));
	//RootComponent = Scene;
	//SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("GoalCollision"));
	//SphereComp->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void ADestructionObject::BeginPlay()
{
	Super::BeginPlay();
	WaterSurface = Cast<AWaterSurface>((UGameplayStatics::GetActorOfClass(GetWorld(), AWaterSurface::StaticClass())));

}

// Called every frame
void ADestructionObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	if ((!WaterSurface->IsInWater(GetActorLocation())))
	{
		SetActorLocation(GetActorLocation() + FVector(0, 0, -2));
		return;
	}

	// ”g‚ÌŒX‚«‚É‰ž‚¶‚ÄˆÚ“®‚·‚é
	FVector WavePower = WaterSurface->GetWavePower(GetActorLocation());
	FVector MoveVec = WavePower * FloatSpeed * 0.01f;
	Velocity += MoveVec;
	if (Velocity.Size() > MinFloatWavePower)
		SetActorLocation(WaterSurface->AdjustMoveInWater(this, Velocity, FloatScale));
	Velocity *= 0.98f;

	// ”g‚Ì‚‚³‚É‡‚í‚¹‚é
	FVector CurPos = GetActorLocation();
	float Height = WaterSurface->GetWaveHeight(CurPos);
	Height = FMath::Lerp(CurPos.Z, Height, 0.1f);
	SetActorLocation(FVector(CurPos.X, CurPos.Y, Height));

}

void ADestructionObject::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	// Õ“Ë‚µ‚½ƒAƒNƒ^[‚ª”š’e‚ÌŽž”š”­‚µ‚ÄƒS[ƒ‹Ï‚Ý‚Å‚àƒS[ƒ‹‚µ‚Ä‚¢‚È‚¢‚±‚Æ‚É‚·‚é
	if (OtherActor->ActorHasTag("Bom"))
	{
		OtherActor->Destroy();
		BreakObject();
		return;
	}

}

