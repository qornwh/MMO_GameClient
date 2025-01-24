// Fill out your copyright notice in the Description page of Project Settings.

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
	// Sets default values for this actor's properties
	ABJS_Bullet();

	FOnBulletCollison OnBulletCollison;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void InitStartDirection(FVector Direction, FVector Pos, int32 SkillCode, int32 AttackNumber = 0);
	void ChangeDirection(FVector TargetDirection = FVector::Zero());

	int32 GetHightSize();
	void SetSkillCode(int32 Code);
	void SetState(TSharedPtr<class BJS_CharaterState> State);
	int32 GetSkillCode() const { return _SkillCode; }
	float GetScaledCapsuleRadius() const { return CollisionComponent->GetScaledCapsuleRadius(); }
	void SetPresentPos();
	FVector GetPresentPos() const { return PresentPos; }
	float GetCollisionWidth() const;
	bool IsCoolDown();
	bool IsMoveProjectileBullet() const { return MoveProjectileBullet; }
	int32 GetTargetCount() const { return TargetCount; }

private:
	TWeakPtr<class BJS_CharaterState> CharaterState;
	int32 _SkillCode = 0;
	int32 _AttackNumber;
	FVector PresentPos = FVector::Zero();
	
	int32 TargetCount;
	int32 AttackCount;
	bool MoveProjectileBullet = false;
	bool IsTargetting;
	float InterVal = 0.2f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
	float Speed = 3000.f;
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack, meta = (AllowPrivateAccess = "true"))
	// bool IsMultipleAttack = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack, meta = (AllowPrivateAccess = "true"))
	float AttackCheckTimer = 1.f;
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = InterVal, meta = (AllowPrivateAccess = "true"))
	// float InterVal = 0.2f;
};
