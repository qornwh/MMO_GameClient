
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BJS_Character.h"
#include "BJS_Monster.generated.h"

UCLASS()
class ARPG_CLIENT_API ABJS_Monster : public ABJS_Character
{
	GENERATED_BODY()

public:
	ABJS_Monster();
	virtual void SetState(TSharedPtr<BJS_CharaterState> state) override;

protected:
	virtual void BeginPlay() override;
	virtual void Move(float DeltaTime) override;
	virtual void Tick(float DeltaTime) override;
	virtual bool PlaySkill(int32 Code, bool ignore = false) override;

	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly)
	class UStaticMeshComponent* MonsterMeshComponent;

	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly)
	class UParticleSystemComponent* ParticleComponent;
	
private:
	TMap<int32, TObjectPtr<class UStaticMesh>> MonsterMeshList;
};
