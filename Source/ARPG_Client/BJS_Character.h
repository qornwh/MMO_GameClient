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
	ABJS_Character();

protected:
	virtual void BeginPlay() override;
	virtual void Move(float DeltaTime);
	virtual void Tick(float DeltaTime) override;
	virtual void Destroyed() override;
	void UpdateBuff(float DeltaTime);
	
public:
	void SetSkeletalMesh(class USkeletalMesh* CurrentMesh);
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	bool GetAim();
	void SetAim(bool Flag);
	void SetAttackTimer(float DeltaTime);
	virtual void StopJump();
	virtual void StartJump();

	void CreateWeaponActor(class USkeletalMesh* WeaponMesh);
	void ChangeWeaponSocket();
	void SetMuzzleFX(class UParticleSystem* MuzzleFX);
	void SetBulletFX(class UNiagaraSystem* SkillFx, int32 Code);
	virtual void Attack();
	virtual void AttackEnd();
	void PlayMotion(int32 Code, bool ignore = false);
	virtual bool PlayAttack(int32 Code, bool ignore = false);
	virtual void PlaySkill(int32 Code, bool ignore = false);
	TSharedPtr<class BJS_CharaterState> GetState();
	virtual void SetState(TSharedPtr<class BJS_CharaterState> state);
	void CreateDamageUi(float Demage);
	void CreateHealUi(float Heal);
	bool GetHitted();
	void LoadInfo(int32 MeshCode, int32 WeaponCode);
	void SetActivate(bool Flag);

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ABJS_DemageActor> DefaultDamgeActor;

protected:
	void UpdateHpWidget();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Widget, meta = (AllowPrivateAccess = "true"))
	class UWidgetComponent* HpBar;
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly)
	TSubclassOf<class AActor> BulletClass;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UBJS_AnimInstance_Base> AnimInstanceClass;

	TObjectPtr<class ABJS_WeaponActor> Weapon;
	TObjectPtr<class UBJS_AnimInstance_Base> AnimInstance;
	TObjectPtr<class UParticleSystem> FireStartFXList;
	TMap<int32, TObjectPtr<class USkeletalMesh>> CharaterMeshList;
	TMap<int32, TObjectPtr<class USkeletalMesh>> WeaponMeshList;
	TMap<int32, TObjectPtr<class UNiagaraSystem>> BulletFXList;
	TSharedPtr<class BJS_CharaterState> State;
	TMap<int32, TObjectPtr<class ABJS_Bullet>> Bullets;
	TMap<int32, CharaterSkill> BuffList;
	TMap<int32, int32> SkillList;

	bool IsAim = false;
	bool IsAttack = true;
	bool IsHitted = false;
	int32 PresentAttackNumber = -1;
	const int32 BulletNumberMax = 20;
	float AttackCheckTime = 0.25f;
	float AttackTimer = -1.f;
};
