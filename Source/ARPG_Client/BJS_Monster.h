// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BJS_Character.h"
#include "GameFramework/Character.h"
#include "BJS_Monster.generated.h"

UCLASS()
class ARPG_CLIENT_API ABJS_Monster : public ABJS_Character
{
	GENERATED_BODY()

public:
	ABJS_Monster();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void Move(float DeltaTime) override;
	
	UFUNCTION()
	void OnHitEvent(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	virtual void SetState(TSharedPtr<BJS_CharaterState> state) override;
	virtual void Attack() override;
	virtual void AttackEnd() override;
	
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly)
	TSubclassOf<class AActor> BulletClass;
};