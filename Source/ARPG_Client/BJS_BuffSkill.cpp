// Fill out your copyright notice in the Description page of Project Settings.


#include "BJS_BuffSkill.h"

#include "Particles/ParticleSystemComponent.h"

// Sets default values
ABJS_BuffSkill::ABJS_BuffSkill()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ParticleComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("MuzzleFX"));
	ParticleComponent->SetupAttachment(RootComponent);
	ParticleComponent->bAutoActivate = false;
	ParticleComponent->SetRelativeScale3D(FVector(1.f, 1.f, 1.f));

	SetActorTickEnabled(false);
}

// Called when the game starts or when spawned
void ABJS_BuffSkill::BeginPlay()
{
	Super::BeginPlay();
	// 파티클 스타트
}

void ABJS_BuffSkill::SetBuffInit(UParticleSystem* Fx, float Duration)
{
	ParticleComponent->SetTemplate(Fx);
	ParticleComponent->Activate(true);
	ParticleComponent->SetComponentTickEnabled(true);
	
	FTimerHandle ParticleTimerHandle;

	UE_LOG(LogTemp, Log, TEXT("time !!! %lf"), Duration);
	if (Duration == 0.f)
		Duration = 0.5f;
	// 일단 타이머로 설정한다
	GetWorld()->GetTimerManager().SetTimer(ParticleTimerHandle, this, &ABJS_BuffSkill::EndBuff, Duration, false);
}

void ABJS_BuffSkill::EndBuff()
{
	ParticleComponent->Deactivate();
	ParticleComponent->SetComponentTickEnabled(false);
	
	Destroy();
}

