// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BJS_BuffSkill.generated.h"

UCLASS()
class ARPG_CLIENT_API ABJS_BuffSkill : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABJS_BuffSkill();
	void SetBuffInit(class UParticleSystem* Fx, float Duration);
	void EndBuff();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	class UParticleSystemComponent* ParticleComponent;

	// 일단 스마트포인터 안쓰고 생포인터로 지정
	// class ABJS_BuffSkill* BuffSkill;
};
