// Fill out your copyright notice in the Description page of Project Settings.


#include "BJS_WeaponActor.h"

#include "BJS_InGameMode.h"
#include "Particles/ParticleSystemComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

// Sets default values
ABJS_WeaponActor::ABJS_WeaponActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	SetRootComponent(SkeletalMeshComponent);

	ParticleComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("MuzzleFX"));
	ParticleComponent->SetupAttachment(RootComponent);
	ParticleComponent->bAutoActivate = false;
	ParticleComponent->SetRelativeScale3D(FVector(0.15f, 0.15f, 0.15f));

	SkillComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("SkillFX"));
	SkillComponent->SetupAttachment(RootComponent);
	SkillComponent->bAutoActivate = false;
}

// Called every frame
void ABJS_WeaponActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABJS_WeaponActor::FireStart()
{
	Fire = true;

	FName WeaponSocket(TEXT("MuzzleFlash"));
	if (SkeletalMeshComponent->DoesSocketExist(WeaponSocket))
	{
		// 파티클 스타트
		if (ParticleComponent && ParticleComponent->Template)
		{
			// 재생중일경우 강제로 처음부터 시작
			ParticleComponent->Deactivate();
			ParticleComponent->SetComponentTickEnabled(false);
			ParticleComponent->Activate(true);
			ParticleComponent->SetComponentTickEnabled(true);
		}
	}
}

void ABJS_WeaponActor::SkillStart()
{
	SkillComponent->Activate(true);
}

bool ABJS_WeaponActor::IsFire()
{
	return Fire;
}

void ABJS_WeaponActor::FireEnd(UNiagaraComponent* FinishedComponent)
{
	Fire = false;
	OnAttackEnd.Execute();
}

void ABJS_WeaponActor::SetSkeletalMesh(TObjectPtr<class USkeletalMesh> CurrentMesh) const
{
	if (SkeletalMeshComponent && CurrentMesh)
	{
		SkeletalMeshComponent->SetSkeletalMesh(CurrentMesh);
	}
}

void ABJS_WeaponActor::SetMuzzleFX(TObjectPtr<class UParticleSystem> Particle) const
{
	if (SkeletalMeshComponent && Particle)
	{
		FName WeaponSocket(TEXT("MuzzleFlash"));
		if (SkeletalMeshComponent->DoesSocketExist(WeaponSocket))
		{
			ParticleComponent->SetTemplate(Particle);
			ParticleComponent->AttachToComponent(SkeletalMeshComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket);
		}
	}
}

void ABJS_WeaponActor::SetBulletFX(TObjectPtr<class UNiagaraSystem> Particle, int32 Code)
{
	if (SkillComponent && Particle)
	{
		FName WeaponSocket(TEXT("MuzzleFlash"));
		if (SkeletalMeshComponent->DoesSocketExist(WeaponSocket))
		{
			SkillComponent->SetAsset(Particle);
			SkillComponent->AttachToComponent(SkeletalMeshComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket);
			SkillComponent->OnSystemFinished.AddDynamic(this, &ABJS_WeaponActor::FireEnd);
			SkillComponent->AddLocalRotation(FRotator(0,180,0));
		}
	}
}

void ABJS_WeaponActor::SetState(TSharedPtr<BJS_CharaterState> State)
{
	CharaterState = State;
}

FVector ABJS_WeaponActor::GetSpawnLocation() const
{
	FName WeaponSocket(TEXT("MuzzleFlash"));
	if (SkeletalMeshComponent->DoesSocketExist(WeaponSocket))
	{
		return SkeletalMeshComponent->GetSocketLocation(WeaponSocket);
	}
	return GetActorLocation();
}
