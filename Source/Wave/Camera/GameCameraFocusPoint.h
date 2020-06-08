#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameCameraFocusPoint.generated.h"

UCLASS()
class WAVE_API AGameCameraFocusPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	AGameCameraFocusPoint();

	static void SpawnFocusPoint(const AActor * Object, const FVector & Pos, float Time);

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	void MyDestroy();
};
