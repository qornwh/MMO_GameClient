// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BJS_ControlCharacter.h"
#include "GameFramework/Actor.h"
#include "BJS_Bullet.generated.h"

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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Destroyed() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void InitStartDirection(FVector Direction, FVector Pos);
	void ChangeDirection(FVector TargetDirection = FVector::Zero());

	UFUNCTION()
	void OnHitEvent(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	int32 GetHightSize();
	int32 GetAttackTime();
	void HitCheck();
	void SetTarget(class ABJS_Character* Target, class ABJS_Character* Parent);
	void SetSkillCode(int32 Code);
	void SetState(TSharedPtr<class BJS_CharaterState> State);

private:
	TWeakPtr<class BJS_CharaterState> CharaterState;
	int32 SkillCode = 0;
	FVector StartPos = FVector::Zero();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
	float Speed = 3000.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack, meta = (AllowPrivateAccess = "true"))
	bool isMultipleAttack = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack, meta = (AllowPrivateAccess = "true"))
	float AttackCheckTimer = 1.f;

	float AttackTimer = 0;
	class ABJS_Character* CurrentTarget;
	class ABJS_Character* ParentTarget;
	bool IsTargeting = false;
};
