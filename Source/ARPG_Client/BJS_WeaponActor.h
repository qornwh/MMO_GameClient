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

	FOnAttackEnd OnAttackEnd;
	
	UFUNCTION()
	void FireEnd(UNiagaraComponent* FinishedComponent);
	
	virtual void Tick(float DeltaTime) override;
	void FireStart();
	void SkillStart();
	bool IsFire();
	void SetSkeletalMesh(TObjectPtr<class USkeletalMesh> CurrentMesh) const;
	void SetMuzzleFX(TObjectPtr<class UParticleSystem> Particle) const;
	void SetBulletFX(TObjectPtr<class UNiagaraSystem> Particle, int32 Code);
	void SetState(TSharedPtr<class BJS_CharaterState> State);
	FVector GetSpawnLocation() const;

private:
	TWeakPtr<class BJS_CharaterState> CharaterState;
	bool Fire = false;
};
