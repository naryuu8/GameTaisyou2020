// ”g‚Ì‰‰Z‚É‚Í”g“®•û’ö®‚ğg—p‚µ‚Ä‚¢‚é
// ˆÈ‰º”g“®•û’ö®‚ÌQlƒTƒCƒg
// https://qiita.com/Ushio/items/0249fd7a5363ccd914dd
// https://www.slideshare.net/UnityTechnologiesJapan/unite-2017-tokyo3d-76689196
// https://www.nicovideo.jp/watch/sm9470923

#include "WaterSurface.h"
#include "CircleLandPoint.h"

AWaterSurface::AWaterSurface()
{
	PrimaryActorTick.bCanEverTick = true;

	for (int yi = 0; yi < SplitVector.Y; ++yi)
	{
		for (int xi = 0; xi < SplitVector.X; ++xi)
		{
			Vertices.Emplace(0, 0, 0);
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

	// ”g‰‰Z—pƒŠƒXƒg‚Ì‰Šú‰»
	CurrentHeights.Init(0.0f, SplitVector.X * SplitVector.Y);
	PrevHeights.Init(0.0f, SplitVector.X * SplitVector.Y);
	NewHeights.Init(0.0f, SplitVector.X * SplitVector.Y);

	for (int i = 0; i < CircleLandPoints.Num(); i++)
	{
		SetCircleLand(CircleLandPoints[i]->GetActorLocation(), CircleLandPoints[i]->GetRadius());
	}

	for (int i = 0; i < LandStartPoints.Num(); i++)
	{
		FVector2D startPos = LocationToVertices(LandStartPoints[i]->GetActorLocation());
		FVector2D endPos = LocationToVertices(LandEndPoints[i]->GetActorLocation());
		SetLand(startPos.X, startPos.Y, endPos.X, endPos.Y);
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

	float c = (DeltaTime * 10.0f) * (DeltaTime * 10.0f);
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

			float k = 0.1f;
			float damp = -k * DeltaTime * (u - u_pre);

			NewHeights[index] = (2.0f * u) + (c * (uL + uR + uT + uB - (4.0f * u))) - u_pre + damp;
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
				Vertices[CalcIndex(xi,yi)].Z += CurrentHeights[CalcIndex(xi,yi)];
		}
	}

	UpdateMesh();
}

void AWaterSurface::CreateWave(int32 x, int32 y)
{
	// ƒKƒEƒX‰‰Z
	auto gauss = [](float x, float sigma)
	{
		return 1.0f / sqrt(PI * 2) * sigma * exp(-x * x / (2.0f * sigma * sigma));
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
			value += gauss(norm, 1.0f) * 100.0f;

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
				Vertices[CalcIndex(xi, yi)].Z = 10.0f;
				UV0[CalcIndex(xi, yi)] = FVector2D((xi / SplitVector.X) * 0.5f + 0.5f, (yi / SplitVector.Y));
			}
		}
	}
}

void AWaterSurface::SetLand(int32 sx, int32 sy, int32 ex, int32 ey)
{
	for (int xi = sx; xi < ex; ++xi)
	{
		for (int yi = sy; yi < ey; ++yi)
		{
			IsLands[CalcIndex(xi, yi)] = true;
			Vertices[CalcIndex(xi, yi)].Z = 10.0f;
			UV0[CalcIndex(xi, yi)] = FVector2D((xi / SplitVector.X) * 0.5f + 0.5f, (yi / SplitVector.Y));
		}
	}
}

void AWaterSurface::AddPower(FVector worldPos)
{
	int32 WaveX = worldPos.X / X_Size;
	int32 WaveY = worldPos.Y / Y_Size;

	CreateWave(WaveX, WaveY);
}