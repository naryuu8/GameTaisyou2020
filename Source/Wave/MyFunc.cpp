// Fill out your copyright notice in the Description page of Project Settings.


#include "MyFunc.h"
#include "Engine/Texture2D.h"
#include "Engine/Engine.h"

// é≤( axis ) âÒì]äpìx( angle( ÉâÉWÉAÉìäp ) )
FQuat MyFunc::FromAxisAngleToQuaternion(const FVector & axis, float angle)
{
	FQuat q;
	float t = FMath::Sin(angle / 2);
	q.X = axis.X * t;
	q.Y = axis.Y * t;
	q.Z = axis.Z * t;
	q.W = FMath::Cos(angle / 2);

	return q;
}

UTexture2D* MyFunc::LoadTexture2DFromAssetPath(const FName AssetPath)
{
	return  LoadObject<UTexture2D>(NULL, *AssetPath.ToString(), NULL, LOAD_None, NULL);
}

void MyFunc::DisplayDebugMessage(const FColor DisplayColor,FString message)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 0.0f, DisplayColor, message);
	}
}

bool MyFunc::ColSegments(const FRay2D & seg1, const FRay2D & seg2, FVector2D * outPos)
{
	FVector2D v = seg2.Origin - seg1.Origin;
	float Crs_v1_v2 = FVector2D::CrossProduct(seg1.Direction, seg2.Direction);
	if (Crs_v1_v2 == 0.0f) {
		// ïΩçsèÛë‘
		return false;
	}

	float Crs_v_v1 = FVector2D::CrossProduct(v, seg1.Direction);
	float Crs_v_v2 = FVector2D::CrossProduct(v, seg2.Direction);

	float t1 = Crs_v_v2 / Crs_v1_v2;
	float t2 = Crs_v_v1 / Crs_v1_v2;

	const float eps = 0.00001f;
	if (t1 + eps < 0 || t1 - eps > 1 || t2 + eps < 0 || t2 - eps > 1) {
		// åç∑ÇµÇƒÇ¢Ç»Ç¢
		return false;
	}

	if (outPos)
		*outPos = seg1.Origin + seg1.Direction * t1;

	return true;
}

bool MyFunc::Check_Ray2D_VS_Ray2D(FRay2DCastInfo & Info, FRay2D RayA, const FRay2D & RayB, const FVector2D & Normal)
{
	FVector2D HitPos;

	if (MyFunc::ColSegments(RayA, RayB, &HitPos))
	{
		if (Info.IsHit)
		{
			float Dist = FVector2D::Distance(RayA.Origin, HitPos);
			if (Dist < Info.HitDist)
			{
				Info.HitDist = Dist;
				Info.NearSideRay = RayB;
				Info.NearNormal = Normal;
				Info.NearPos = HitPos;
			}
		}
		else
		{
			Info.IsHit = true;
			Info.HitDist = FVector2D::Distance(RayA.Origin, HitPos);
			Info.NearSideRay = RayB;
			Info.NearNormal = Normal;
			Info.NearPos = HitPos;
		}
		return true;
	}
	return false;
}

bool MyFunc::Check_CircleRay2D_VS_Ray2D(FRay2DCastInfo & Info, FRay2D RayA, float CircleRadius, const FRay2D & RayB, const FVector2D & Normal)
{
	FVector2D HitPos;
	FVector2D CorrectionPos = CircleRadius * Normal;
	RayA.Direction -= CorrectionPos;	// â~ÇÃï‚ê≥ï™Ç∏ÇÁÇ∑

	if (MyFunc::ColSegments(RayA, RayB, &HitPos))
	{
		if (Info.IsHit)
		{
			float Dist = FVector2D::Distance(RayA.Origin, HitPos);
			if (Dist < Info.HitDist)
			{
				Info.HitDist = Dist;
				Info.NearSideRay = RayB;
				Info.NearNormal = Normal;
				Info.NearPos = HitPos + CorrectionPos;
			}
		}
		else
		{
			Info.IsHit = true;
			Info.HitDist = FVector2D::Distance(RayA.Origin, HitPos);
			Info.NearSideRay = RayB;
			Info.NearNormal = Normal;
			Info.NearPos = HitPos + CorrectionPos;
		}
		return true;
	}
	return false;
}
