// îgÇÃââéZÇ…ÇÕîgìÆï˚íˆéÆÇégópÇµÇƒÇ¢ÇÈ
// à»â∫îgìÆï˚íˆéÆÇÃéQçlÉTÉCÉg
// https://qiita.com/Ushio/items/0249fd7a5363ccd914dd
// https://www.slideshare.net/UnityTechnologiesJapan/unite-2017-tokyo3d-76689196
// https://www.nicovideo.jp/watch/sm9470923

#include "WaterSurface.h"
#include "Kismet/GameplayStatics.h"
#include "CircleLand.h"
#include "SquareLand.h"
#include "FlashFlood.h"

AWaterSurface::AWaterSurface()
{
	PrimaryActorTick.bCanEverTick = true;

	for (int yi = 0; yi < SplitVector.Y; ++yi)
	{
		for (int xi = 0; xi < SplitVector.X; ++xi)
		{
			Vertices.Emplace(0, 0, 0);
			VertexColors.Emplace(WaterColor);
			UV0.Emplace((xi / SplitVector.X) * 0.5f , (yi / SplitVector.Y));
		}
	}

	IsLands.Init(false, Vertices.Num());

	for (int yi = 0; yi < SplitVector.Y - 1; yi++)
	{
		for (int xi = 0; xi < SplitVector.X - 1; xi++)
		{
			Triangles.Emplace((SplitVector.Y * (yi + 0)) + xi);
			Triangles.Emplace((SplitVector.Y * (yi + 1)) + xi);
			Triangles.Emplace((SplitVector.Y * (yi + 0)) + xi + 1);
			Triangles.Emplace((SplitVector.Y * (yi + 0)) + xi + 1);
			Triangles.Emplace((SplitVector.Y * (yi + 1)) + xi);
			Triangles.Emplace((SplitVector.Y * (yi + 1)) + xi + 1);
		}
	}
	CreateMesh();
}

void AWaterSurface::BeginPlay()
{
	Super::BeginPlay();

	X_Size = (-StartPoint->GetActorLocation().X + EndPoint->GetActorLocation().X) / SplitVector.X;
	Y_Size = (-StartPoint->GetActorLocation().Y + EndPoint->GetActorLocation().Y) / SplitVector.Y;

	for (int yi = 0; yi < SplitVector.Y; ++yi)
	{
		for (int xi = 0; xi < SplitVector.X; ++xi)
		{
			Vertices[CalcIndex(xi, yi)].X = X_Size * xi;
			Vertices[CalcIndex(xi, yi)].Y = Y_Size * yi;
		}
	}

	// îgââéZópÉäÉXÉgÇÃèâä˙âª
	CurrentHeights.Init(0.0f, SplitVector.X * SplitVector.Y);
	PrevHeights.Init(0.0f, SplitVector.X * SplitVector.Y);
	NewHeights.Init(0.0f, SplitVector.X * SplitVector.Y);

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ALandPoint::StaticClass(), FoundActors);

	for (auto Actor : FoundActors)
	{
		ACircleLand* CircleLand = Cast<ACircleLand>(Actor);
		if (CircleLand)
		{
			SetCircleLand(CircleLand->GetActorLocation(), CircleLand->GetRadius());
		}
	}

	for (auto Actor : FoundActors)
	{
		ASquareLand* SquareLand = Cast<ASquareLand>(Actor);
		if (SquareLand)
		{
			SetSquareLand(SquareLand->GetActorLocation(), SquareLand->GetXLength(), SquareLand->GetYLength());
		}
	}

	if (Material)
	{
		Mesh->SetMaterial(0, Material);
	}

	UpdateMesh();
}

void AWaterSurface::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float DeltaSpeed = WaveSpeed / 60;

	float c = 2.0f;
	float mul = DeltaSpeed * DeltaSpeed * c * c / (SplitVector.X * SplitVector.Y) * (SplitVector.X * SplitVector.Y);
	for (int xi = 1; xi < SplitVector.X - 1; ++xi)
	{
		for (int yi = 1; yi < SplitVector.Y - 1; ++yi)
		{
			int32 index = CalcIndex(xi, yi);

			float uL = CurrentHeights[this->CalcIndex(xi - 1, yi)];
			float uR = CurrentHeights[this->CalcIndex(xi + 1, yi)];
			float uT = CurrentHeights[this->CalcIndex(xi, yi - 1)];
			float uB = CurrentHeights[this->CalcIndex(xi, yi + 1)];

			float u_pre = PrevHeights[index];
			float u = CurrentHeights[index];

			float damp = Decay * DeltaSpeed * (u - u_pre);

			NewHeights[index] = u + u - u_pre + (mul * (uL + uR + uT + uB - (4.0f * u))) - damp;
		}
	}

	for (int xi = 1; xi < SplitVector.X - 1; ++xi)
	{
		for (int yi = 1; yi < SplitVector.Y - 1; ++yi)
		{
			int32 index = CalcIndex(xi, yi);
			PrevHeights[index] = CurrentHeights[index];
			CurrentHeights[index] = NewHeights[index];
		}
	}

	for (int yi = 0; yi < SplitVector.X; ++yi)
	{
		for (int xi = 0; xi < SplitVector.Y; ++xi)
		{
			if(!IsLands[CalcIndex(xi,yi)])
				Vertices[CalcIndex(xi,yi)].Z = CurrentHeights[CalcIndex(xi,yi)];

			if (Vertices[CalcIndex(xi, yi)].Z > MaxWaveHight)
				Vertices[CalcIndex(xi, yi)].Z = MaxWaveHight;

			VertexColors[CalcIndex(xi, yi)] = FLinearColor::LerpUsingHSV(WaterColor, WaveColor, Vertices[CalcIndex(xi, yi)].Z / MaxWaveHight);
		}
	}

	UpdateMesh();
}

void AWaterSurface::CreateWave(int32 x, int32 y, float power)
{
	// ÉKÉEÉXââéZ
	auto gauss = [](float distance, float sigma)
	{
		return 1.0f / sqrt(PI * 2) * sigma * exp(-distance * distance / (2.0f * sigma * sigma));
	};

	FVector2D wv = FVector2D(x, y);

	for (int xi = 1; xi < SplitVector.X - 1; ++xi)
	{
		for (int yi = 1; yi < SplitVector.Y - 1; ++yi)
		{
			int index = CalcIndex(xi, yi);
			float value = 0.0f;

			FVector2D v = FVector2D(xi, yi);

			float norm = FVector2D::Distance(v, wv);
			value += gauss(norm, 3.0f) * power;

			CurrentHeights[index] += value;
			PrevHeights[index] += value;
			NewHeights[index] += value;
		}
	}
}

FVector2D AWaterSurface::LocationToVertices(FVector Location)
{
	int xi = (Location.X - GetActorLocation().X) / X_Size;
	int yi = (Location.Y - GetActorLocation().Y) / Y_Size;
	return FVector2D(xi, yi);
}

int32 AWaterSurface::CalcIndex(int32 x, int32 y)
{
	return x + (y * SplitVector.X);
}

void AWaterSurface::SetCircleLand(FVector CirclePostion, float Radius)
{
	for (int xi = 1; xi < SplitVector.X - 1; ++xi)
	{
		for (int yi = 1; yi < SplitVector.Y - 1; ++yi)
		{
			float xp = Vertices[CalcIndex(xi, yi)].X;
			float yp = Vertices[CalcIndex(xi, yi)].Y;
			float xc = CirclePostion.X;
			float yc = CirclePostion.Y;
			if ((xp - xc)*(xp - xc) + (yp - yc)*(yp - yc) <= Radius * Radius)
			{
				IsLands[CalcIndex(xi, yi)] = true;
				Vertices[CalcIndex(xi, yi)].Z = CirclePostion.Z;
				UV0[CalcIndex(xi, yi)] = FVector2D((xi / SplitVector.X) * 0.5f + 0.5f, (yi / SplitVector.Y));
			}
		}
	}
}

void AWaterSurface::SetSquareLand(FVector SquareLocation, float XLength, float YLength)
{
	for (int xi = 1; xi < SplitVector.X - 1; ++xi)
	{
		for (int yi = 1; yi < SplitVector.Y - 1; ++yi)
		{
			float xp = Vertices[CalcIndex(xi, yi)].X;
			float yp = Vertices[CalcIndex(xi, yi)].Y;
			float xs = SquareLocation.X;
			float ys = SquareLocation.Y;

			if (xp > xs + XLength * 0.5f) continue;
			if (xp < xs - XLength * 0.5f) continue;
			if (yp > ys + YLength * 0.5f) continue;
			if (yp < ys - YLength * 0.5f) continue;

			IsLands[CalcIndex(xi, yi)] = true;
			Vertices[CalcIndex(xi, yi)].Z = SquareLocation.Z;
			UV0[CalcIndex(xi, yi)] = FVector2D((xi / SplitVector.X) * 0.5f + 0.5f, (yi / SplitVector.Y));
		}
	}
}

void AWaterSurface::AddPower(FVector worldPos, float power = 100.0f)
{
	int32 WaveX = worldPos.X / X_Size;
	int32 WaveY = worldPos.Y / Y_Size;

	//if (WaveX <= 0) return;
	//if (WaveX > SplitVector.X) return;
	//if (WaveY <= 0) return;
	//if (WaveY > SplitVector.Y) return;

	CreateWave(WaveX, WaveY, power);
}

FVector AWaterSurface::GetWavePower(FVector worldPos)
{
	if (!IsInWater(worldPos)) return FVector::ZeroVector;

	FVector answerVec = FVector::ZeroVector;

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFlashFlood::StaticClass(), FoundActors);

	for (auto Actor : FoundActors)
	{
		AFlashFlood* FlashFlood = Cast<AFlashFlood>(Actor);
		if (FlashFlood)
		{
			FVector addVec = FlashFlood->GetFloatVec(worldPos);
			addVec.Z = 0;
			answerVec += addVec;
		}
	}

	int32 WaveX = worldPos.X / X_Size;
	int32 WaveY = worldPos.Y / Y_Size;

	if (WaveX <= 2) return answerVec;
	if (WaveY <= 2) return answerVec;
	if (WaveX >= SplitVector.X - 3) return answerVec;
	if (WaveY >= SplitVector.Y - 3) return answerVec;
	
	float uL = CurrentHeights[CalcIndex(WaveX - 1, WaveY)];
	float uR = CurrentHeights[CalcIndex(WaveX + 1, WaveY)];
	float uT = CurrentHeights[CalcIndex(WaveX, WaveY - 1)];
	float uB = CurrentHeights[CalcIndex(WaveX, WaveY + 1)];

	float x = uL - uR;
	float y = uT - uB;

	answerVec += FVector(x, y, 0);

	return answerVec;
}

FVector AWaterSurface::GetOutLandPos(FVector worldPos, float circleRadius)
{
	if (!IsLand(worldPos)) return worldPos;

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ALandPoint::StaticClass(), FoundActors);

	for (auto Actor : FoundActors)
	{
		ACircleLand* CircleLand = Cast<ACircleLand>(Actor);
		if (!CircleLand) continue;

		float distance = FVector::Distance(worldPos, CircleLand->GetActorLocation());
		float judgDistance = circleRadius + CircleLand->GetRadius();
		float landingDistance = judgDistance - distance;
		if (landingDistance <= 0) continue;

		FVector outDirection = worldPos - CircleLand->GetActorLocation();
		outDirection.Z = 0;
		outDirection.Normalize();

		worldPos = worldPos + outDirection * landingDistance;

		if (!IsLand(worldPos)) return worldPos;
	}

	for (auto Actor : FoundActors)
	{
		ASquareLand* SquareLand = Cast<ASquareLand>(Actor);
		if (!SquareLand) continue;

		FVector judgVec = FVector::ZeroVector;
		FVector moveVec = FVector::ZeroVector;
		OBB2D obb = SquareLand->GetOBB();
		for (int i = 0; i < 2; i++)
		{
			float length = obb.GetLength(i);
			if (length <= 0) continue;
			float dot = FVector::DotProduct(worldPos - obb.GetPosition(), obb.GetDirecton(i)) / length;
			dot = fabs(dot);
			if (dot > 1)
				judgVec += (1 - dot) * obb.GetDirecton(i) * length;

			if ((dot - 1) * length <= circleRadius)
				moveVec += obb.GetDirecton(i) * (((dot - 1) * length) - circleRadius);

		}

		if (judgVec.Size() >= circleRadius) continue;

		moveVec.Y = 0;
		worldPos = worldPos + moveVec;

		if (!IsLand(worldPos)) return worldPos;
	}

	return worldPos;
}

FVector AWaterSurface::AdjustMoveInLand(FVector actorPos, FVector moveVec, float circleRadius)
{
	FVector movedPos = actorPos + moveVec;

	for (int xi = 0; xi < SplitVector.X; ++xi)
	{
		for (int yi = 0; yi < SplitVector.Y; ++yi)
		{
			float xp = Vertices[CalcIndex(xi, yi)].X;
			float yp = Vertices[CalcIndex(xi, yi)].Y;
			float xc = movedPos.X;
			float yc = movedPos.Y;
			if ((xp - xc)*(xp - xc) + (yp - yc)*(yp - yc) <= circleRadius * circleRadius)
			{
				if (!IsLands[CalcIndex(xi, yi)]) return actorPos - moveVec * 0.1f;
			}
		}
	}

	return movedPos;
}


FVector AWaterSurface::AdjustMoveInWater(FVector actorPos, FVector moveVec, float circleRadius)
{
	FVector movedPos = actorPos + moveVec;
	if (!IsLand(movedPos)) return movedPos;

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ALandPoint::StaticClass(), FoundActors);

	for (auto Actor : FoundActors)
	{
		ACircleLand* CircleLand = Cast<ACircleLand>(Actor);
		if (!CircleLand) continue;

		float distance = FVector::Distance(movedPos, CircleLand->GetActorLocation());
		float judgDistance = circleRadius + CircleLand->GetRadius();
		float landingDistance = judgDistance - distance;
		if (landingDistance <= 0) continue;

		FVector outDirection = movedPos - CircleLand->GetActorLocation();
		outDirection.Z = 0;
		outDirection.Normalize();

		movedPos = movedPos + outDirection * landingDistance;

		if (!IsLand(movedPos)) return movedPos;
	}

	for (auto Actor : FoundActors)
	{
		ASquareLand* SquareLand = Cast<ASquareLand>(Actor);
		if (!SquareLand) continue;

		float XDistance = fabs(SquareLand->GetActorLocation().X - movedPos.X);
		float YDistance = fabs(SquareLand->GetActorLocation().Y - movedPos.Y);
		float JudgXDistance = circleRadius + SquareLand->GetXLength() * 0.5f;
		float JudgYDistance = circleRadius + SquareLand->GetYLength() * 0.5f;
		if (JudgXDistance > XDistance)
			movedPos.X = actorPos.X;
		if (JudgYDistance > YDistance)
			movedPos.Y = actorPos.Y;

		if (!IsLand(movedPos)) return movedPos;
	}

	return actorPos;
}

bool AWaterSurface::IsInWater(FVector worldPos)
{
	int32 WaveX = worldPos.X / X_Size;
	int32 WaveY = worldPos.Y / Y_Size;

	int index = CalcIndex(WaveX, WaveY);

	if (index <= 0) return false;
	if (index >= Vertices.Num()) return false;

	return true;
}

bool AWaterSurface::IsLand(FVector worldPos)
{
	if (!IsInWater(worldPos)) return false;

	int32 WaveX = worldPos.X / X_Size;
	int32 WaveY = worldPos.Y / Y_Size;

	int index = CalcIndex(WaveX, WaveY);

	return IsLands[index];
}

FVector AWaterSurface::GetGetOffPos(FVector WorldPos, float Radius)
{
	for (int xi = 1; xi < SplitVector.X - 1; ++xi)
	{
		for (int yi = 1; yi < SplitVector.Y - 1; ++yi)
		{
			if (!IsLands[CalcIndex(xi, yi)]) continue;

			float xp = Vertices[CalcIndex(xi, yi)].X;
			float yp = Vertices[CalcIndex(xi, yi)].Y;
			float xc = WorldPos.X;
			float yc = WorldPos.Y;
			if ((xp - xc)*(xp - xc) + (yp - yc)*(yp - yc) <= Radius * Radius)
			{
				return Vertices[CalcIndex(xi,yi)];
			}
		}
	}

	return FVector::ZeroVector;
}