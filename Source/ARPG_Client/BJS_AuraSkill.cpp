#include "BJS_AuraSkill.h"
#include "NiagaraComponent.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ABJS_AuraSkill::ABJS_AuraSkill()
{
	PrimaryActorTick.bCanEverTick = true;

	ParticleComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleComponent"));
	ParticleComponent->SetupAttachment(RootComponent);
	ParticleComponent->bAutoActivate = false;
	ParticleComponent->SetRelativeScale3D(FVector(1.f, 1.f, 1.f));

	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
	NiagaraComponent->SetupAttachment(RootComponent);
	NiagaraComponent->bAutoActivate = false;
	NiagaraComponent->SetRelativeScale3D(FVector(1.f, 1.f, 1.f));

	SetActorTickEnabled(false);
}

void ABJS_AuraSkill::BeginPlay()
{
	Super::BeginPlay();
}

void ABJS_AuraSkill::SetBuffParticleInit(TObjectPtr<class UParticleSystem> Fx, float Duration)
{
	ParticleComponent->SetTemplate(Fx);
	ParticleComponent->Activate(true);
	ParticleComponent->SetComponentTickEnabled(true);
	
	FTimerHandle ParticleTimerHandle;
	if (Duration == 0.f)
		Duration = 0.5f;
	GetWorld()->GetTimerManager().SetTimer(ParticleTimerHandle, this, &ABJS_AuraSkill::EndBuffParticle, Duration, false);
}

void ABJS_AuraSkill::EndBuffParticle()
{
	ParticleComponent->Deactivate();
	ParticleComponent->SetComponentTickEnabled(false);

	Destroy();
}

void ABJS_AuraSkill::SetBuffNiagraInit(TObjectPtr<class UNiagaraSystem> Fx, float Duration)
{
	NiagaraComponent->SetAsset(Fx);
	NiagaraComponent->Activate(true);
	NiagaraComponent->SetComponentTickEnabled(true);
	
	FTimerHandle ParticleTimerHandle;
	if (Duration == 0.f)
		Duration = 0.5f;
	GetWorld()->GetTimerManager().SetTimer(ParticleTimerHandle, this, &ABJS_AuraSkill::EndBuffParticle, Duration, false);
}

void ABJS_AuraSkill::EndBuffNiagra()
{
	NiagaraComponent->Deactivate();
	NiagaraComponent->SetComponentTickEnabled(false);
	
	Destroy();
}

