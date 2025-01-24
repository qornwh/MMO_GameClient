// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BJS_AnimInstance_Base.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_CLIENT_API UBJS_AnimInstance_Base : public UAnimInstance
{
	GENERATED_BODY()

public: 
	UBJS_AnimInstance_Base();

	virtual void NativeBeginPlay() override;
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	void PlayAttackAM();
	void PauseAttackAM();
	void ResumePauseAM();
	void StopAttackAM();

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	float CurrentDirection;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	float GroundSpeed;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	float VelocityZ;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	bool IsMove;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	bool IsJumping;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	class UAnimMontage* AM_Fire;
	
private:
	// 에임모드
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	bool IsAim;

	class ABJS_Character* ParentPawn;
};
