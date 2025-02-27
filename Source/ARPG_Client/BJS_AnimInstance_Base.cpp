// Fill out your copyright notice in the Description page of Project Settings.


#include "BJS_AnimInstance_Base.h"

#include "BJS_Character.h"
#include "KismetAnimationLibrary.h"
#include "GameFramework/PawnMovementComponent.h"

UBJS_AnimInstance_Base::UBJS_AnimInstance_Base()
{
	GroundSpeed = 0.f;
	CurrentDirection = 0.f;
	VelocityZ = 0.f;
	IsMove = false;
	IsJumping = false;
	IsAim = false;
	ParentPawn = nullptr;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AM_FireObj(TEXT("/Script/Engine.AnimMontage'/Game/MyGame/Assets/Charater/Animation/AM_Fire.AM_Fire'"));
	if (AM_FireObj.Succeeded())
	{
		AM_Fire = AM_FireObj.Object;
	}
}

void UBJS_AnimInstance_Base::NativeBeginPlay()
{
	Super::NativeBeginPlay();
}

void UBJS_AnimInstance_Base::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UBJS_AnimInstance_Base::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!ParentPawn)
	{
		auto pawn = TryGetPawnOwner();
		if (pawn)
			ParentPawn = Cast<ABJS_Character>(pawn);
	}
	
	if (::IsValid(ParentPawn))
	{
		IsAim = ParentPawn->GetAim();
		GroundSpeed = ParentPawn->GetVelocity().Size();
		IsJumping = ParentPawn->GetMovementComponent()->IsFalling();
		VelocityZ = ParentPawn->GetVelocity().Z;
		
		if (IsAim)
		{
			float directPinch = ParentPawn->GetControlRotation().Pitch;
			if (directPinch > 180)
				directPinch -= 360;
			CurrentDirection = directPinch;
		}
		else
		{
			CurrentDirection = UKismetAnimationLibrary::CalculateDirection(
				ParentPawn->GetVelocity(), ParentPawn->GetActorRotation());
		}
		
		if (!IsJumping)
		{
			if (GroundSpeed > 0.f)
			{
				IsMove = true;
			}
			else
			{
				IsMove = false;
			}
		}
		else
		{
			IsMove = false;
		}
		
		if (Montage_IsPlaying(AM_Fire))
		{
			IsMove = false;
		}
	}
}

void UBJS_AnimInstance_Base::PlayAttackAM()
{
	Montage_Play(AM_Fire, 1.f);
}

void UBJS_AnimInstance_Base::PauseAttackAM()
{
	Montage_Pause(AM_Fire);
}

void UBJS_AnimInstance_Base::ResumePauseAM()
{
	Montage_Resume(AM_Fire);
}

void UBJS_AnimInstance_Base::StopAttackAM()
{
	Montage_Stop(0.1f, AM_Fire);
}
