// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BJS_StaticPawn.h"
#include "BJS_LoginPawn.generated.h"

UCLASS()
class ARPG_CLIENT_API ABJS_LoginPawn : public ABJS_StaticPawn
{
	GENERATED_BODY()

public:
	ABJS_LoginPawn();
	virtual void BeginPlay() override;
	virtual void Destroyed() override;

	void CreateWeaponActor(USkeletalMesh* WeaponMesh);
	virtual void SetVisible(bool Flag) override;

private:
	TObjectPtr<class UAnimInstance> LoginPawnAnimInstance;
	TObjectPtr<class ABJS_WeaponActor> Pistol;
};
