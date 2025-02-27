#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BJS_AuraSkill.generated.h"

DECLARE_DELEGATE(FOnAuraSkill)

UCLASS()
class ARPG_CLIENT_API ABJS_AuraSkill : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABJS_AuraSkill();
	FOnAuraSkill OnAuraSkill;
	
	void SetBuffParticleInit(TObjectPtr<class UParticleSystem> Fx, float Duration);
	void EndBuffParticle();
	void SetBuffNiagraInit(TObjectPtr<class UNiagaraSystem> Fx, float Duration);
	void EndBuffNiagra();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	class UParticleSystemComponent* ParticleComponent;
	class UNiagaraComponent* NiagaraComponent;
};
