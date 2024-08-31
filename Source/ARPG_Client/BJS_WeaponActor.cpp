// Fill out your copyright notice in the Description page of Project Settings.


#include "BJS_WeaponActor.h"

#include "BJS_Bullet.h"
#include "BJS_GameInstance.h"
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
	// 대처 필요!!
	AttackTimer = -1.f;
}

// Called when the game starts or when spawned
void ABJS_WeaponActor::BeginPlay()
{
	Super::BeginPlay();

	auto mode = Cast<UBJS_GameInstance>(GetGameInstance());
	if (mode)
	{
		BulletClass = mode->GetSkillBulletMap()[0];		
	}
}

void ABJS_WeaponActor::Destroyed()
{
	Super::Destroyed();

	UE_LOG(LogTemp, Log, TEXT("Destory Bullet !!!"));
}

// Called every frame
void ABJS_WeaponActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (AttackTimer < 0) return;
	
	AttackTimer += DeltaTime;
	if (AttackTimer >= AttackCheckTime)
	{
		AttackTimer = -1;
		FireEnd(nullptr) ;
	}
}

void ABJS_WeaponActor::FireStart(FVector Direction, int32 Code)
{
	Fire = true;

	FName WeaponSocket(TEXT("MuzzleFlash"));
	if (SkeletalMeshComponent->DoesSocketExist(WeaponSocket))
	{
		FVector SpawnLocation = SkeletalMeshComponent->GetSocketLocation(WeaponSocket);
		if (Code == 0)
		{
			auto Bullet = GetWorld()->SpawnActor<ABJS_Bullet>(BulletClass, SpawnLocation, FRotator::ZeroRotator);
			Bullet->InitStartDirection(Direction, SpawnLocation);
			AttackTimer = 0;
			AttackCheckTime = 0.25f;
		}
		else
		{
			auto Bullet = GetWorld()->SpawnActor<ABJS_Bullet>(SkillClass, SpawnLocation, FRotator::ZeroRotator);
			Bullet->InitStartDirection(FVector::ZeroVector, SpawnLocation);
			Bullet->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket);
			Bullet->SetActorRelativeRotation(FRotator(90, 0, 0));
			Bullet->SetActorRelativeLocation(FVector(Bullet->GetHightSize(), 0, 0));
			Bullet->SetSkillCode(Code);
			SkillComponent->Activate(true);
		}

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

bool ABJS_WeaponActor::IsFire()
{
	return Fire;
}

void ABJS_WeaponActor::FireEnd(UNiagaraComponent* FinishedComponent)
{
	Fire = false;
	OnAttackEnd.Execute();
}

void ABJS_WeaponActor::SetSkeletalMesh(USkeletalMesh* CurrentMesh)
{
	if (SkeletalMeshComponent && CurrentMesh)
	{
		SkeletalMeshComponent->SetSkeletalMesh(CurrentMesh);
	}
}

void ABJS_WeaponActor::SetMuzzleFX(UParticleSystem* Particle)
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

void ABJS_WeaponActor::SetBulletFX(UNiagaraSystem* Particle, int32 Code)
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
	
	auto mode = Cast<UBJS_GameInstance>(GetGameInstance());
	if (mode)
	{
		SkillClass = mode->GetSkillBulletMap()[Code];		
	}

	check(SkillClass);
}
