// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BJS_CharaterSkill.h"
#include "GameFramework/Character.h"
#include "BJS_Character.generated.h"

UCLASS()
class ARPG_CLIENT_API ABJS_Character : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABJS_Character();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Move(float DeltaTime);
	void UpdateBuff(float DeltaTime);
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	void SetSkeletalMesh(class USkeletalMesh* CurrentMesh);
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	bool GetAim();
	void SetAim(bool flag);
	virtual void StopJump();
	virtual void StartJump();

	void CreateWeaponActor(class USkeletalMesh* WeaponMesh);
	void ChangeWeaponSocket();
	void SetMuzzleFX(class UParticleSystem* MuzzleFX);
	void SetBulletFX(class UNiagaraSystem* SkillFx, int32 Code);
	virtual void Attack();
	virtual void AttackEnd();
	void PlayMotion(int32 Code, bool ignore = false);
	void PlayAttack(int32 Code, bool ignore = false);
	void PlaySkill(int32 Code, bool ignore = false);
	TSharedPtr<class BJS_CharaterState> GetState();
	virtual void SetState(TSharedPtr<class BJS_CharaterState> state);
	void TakeDemage(float Demage, class ABJS_Character* Target = nullptr);
	void TakeHeal(float Heal);
	void SetTarget(int32 UUid);
	int32 GetTarget();
	bool GetHitted();
	void LoadInfo(int32 MeshCode, int32 WeaponCode);

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AActor> DefaultDamgeActor;

protected:
	void UpdateHpWidget();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Widget, meta = (AllowPrivateAccess = "true"))
	class UWidgetComponent* HpBar;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UBJS_AnimInstance_Base> AnimInstanceClass;

	TObjectPtr<class ABJS_WeaponActor> Weapon;
	TObjectPtr<class UBJS_AnimInstance_Base> AnimInstance;

	TMap<int32, TObjectPtr<class USkeletalMesh>> CharaterMeshList;
	TMap<int32, TObjectPtr<class USkeletalMesh>> WeaponMeshList;
	TMap<int32, TObjectPtr<class UNiagaraSystem>> BulletFXList;
	TMap<int32, int32> SkillList;
	TSharedPtr<class BJS_CharaterState> State;
	TObjectPtr<class UParticleSystem> FireStartFXList;

	bool IsAim = false;
	bool IsAttack = true;

	int32 TargetUUid = -1;
	bool IsHitted = false;

	TMap<int32, CharaterSkill> BuffList;
};
