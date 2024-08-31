// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BJS_DemageActor.generated.h"

UCLASS()
class ARPG_CLIENT_API ABJS_DemageActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABJS_DemageActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetDemage(int32 Demage);
	void SetHeal(int32 Heal);
	void SetRandomLoc();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USceneComponent* RootSceneComponent;
	
	UPROPERTY(VisibleAnywhere)
	class UWidgetComponent* DemageWidget;

	UPROPERTY(EditAnywhere)
	float TimeOut = 0.5f;

	bool OnOff = false;
	float TickTime = 0.5f;
};
