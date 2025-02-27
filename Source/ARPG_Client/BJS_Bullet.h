#pragma once

#include "BJS_ControlCharacter.h"
#include "CoreMinimal.h"
#include "SkillStruct.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Actor.h"
#include "BJS_Bullet.generated.h"

DECLARE_DELEGATE_OneParam(FOnBulletCollison, int32)

UCLASS()
class ARPG_CLIENT_API ABJS_Bullet : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UCapsuleComponent* CollisionComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UProjectileMovementComponent* ProjectileMovementComponent;

public:
	ABJS_Bullet();
	FOnBulletCollison OnBulletCollison;
	
	void InitStartDirection(FVector Direction, FVector Pos, int32 Code, int32 Number = 0);
	void ChangeDirection(FVector TargetDirection = FVector::Zero());
	int32 GetHightSize();
	void SetSkillCode(int32 Code);
	void SetState(TSharedPtr<class BJS_CharaterState> State);
	int32 GetSkillCode() const { return SkillCode; }
	float GetScaledCapsuleRadius() const { return CollisionComponent->GetScaledCapsuleRadius(); }
	void SetPresentPos();
	FVector GetPresentPos() const { return PresentPos; }
	float GetCollisionWidth() const;
	bool IsCoolDown();
	bool IsMoveProjectileBullet() const { return MoveProjectileBullet; }
	int32 GetTargetCount() const { return TargetCount; }
	bool AttackAccessCharater(int32 Code);

protected:
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;

private:
	TWeakPtr<class BJS_CharaterState> CharaterState;
	int32 SkillCode = 0;
	int32 AttackNumber;
	FVector PresentPos = FVector::Zero();
	int32 TargetCount;
	int32 AttackCount;
	bool MoveProjectileBullet = false;
	bool IsTargetting;
	float InterVal = 0.2f;
	TSet<int32> AccessAttacks;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
	float Speed = 10000.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack, meta = (AllowPrivateAccess = "true"))
	float AttackCheckTimer = 1.f;
};
