#pragma once

#include "CoreMinimal.h"

class ARPG_CLIENT_API BulletCollisionUtils
{
public:
	static bool CheckAABB2D(const FVector& A, const FVector& B, const FVector& Target, float dLen)
	{
		dLen = FMath::Abs(dLen);
		if (FMath::Min(A.X, B.X) - dLen <= Target.X && Target.X <= FMath::Max(A.X, B.X) + dLen)
		{
			if (FMath::Min(A.Y, B.Y) - dLen <= Target.Y && Target.Y <=  FMath::Max(A.Y, B.Y) + dLen)
			{
				return true;
			}
		}
		return false;
	}

	static float CapsuleToCircleDistance2D(FVector A, FVector B, FVector Target)
	{
		A.Z = 0;
		B.Z = 0;
		Target.Z = 0;
		
		FVector AToB = B - A;
		FVector AToTarget = Target - A;

		float dot = FVector::DotProduct(AToTarget, AToB);
		float let2sqrt = AToB.SquaredLength();
		float t = dot / let2sqrt;
		float d;

		if (t < 0.f)
		{
			d = FVector(Target - A).Length();
		}
		else if (t > 1.f)
		{
			d = FVector(Target - B).Length();
		}
		else
		{
			FVector h(A.X + t * AToB.X, A.Y + t * AToB.Y, A.Z);
			d = (Target - h).Length();
		}

		return d;
	}
};
