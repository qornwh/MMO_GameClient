// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BJS_ControlCharacter.h"
#include "GameFramework/Actor.h"
#include "BJS_WeaponActor.generated.h"

DECLARE_DELEGATE(FOnAttackEnd);

UCLASS()
class ARPG_CLIENT_API ABJS_WeaponActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABJS_WeaponActor();

	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly)
	class USkeletalMeshComponent* SkeletalMeshComponent;

	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly)
	class UParticleSystemComponent* ParticleComponent;

	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly)
	class UNiagaraComponent* SkillComponent;
	
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly)
	TSubclassOf<class AActor> BulletClass;
	
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly)
	TSubclassOf<class AActor> SkillClass;

	FOnAttackEnd OnAttackEnd;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Destroyed() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void FireStart(FVector Direction, int32 Code = 0);
	bool IsFire();

	UFUNCTION()
	void FireEnd(UNiagaraComponent* FinishedComponent);

	void SetSkeletalMesh(class USkeletalMesh* CurrentMesh);
	void SetMuzzleFX(class UParticleSystem* Particle);
	void SetBulletFX(class UNiagaraSystem* Particle, int32 Code);

private:
	bool Fire = false;

	float AttackCheckTime = 0.25f;
	float AttackTimer = 0.f;
};
