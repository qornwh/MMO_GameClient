// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BJS_Character.h"
#include "BJS_DemageActor.h"
#include "GameFramework/GameModeBase.h"
#include "BJS_GameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_CLIENT_API ABJS_GameModeBase : public AGameModeBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void Destroyed() override;
	virtual void LoadSubClass();
	TSubclassOf<AActor> SocketClass;
	class ABJS_SocketActor* SocketActor;
	class APlayerStart* PlayerStart;

// private:
	// 일단 오브젝트 풀링을 사용한다.
	TMap<UClass*, TArray<TObjectPtr<AActor>>> ActorPool;

public:
	ABJS_GameModeBase();
	class ABJS_SocketActor* GetSocketActor();

	class UBJS_UserWidgetBase* GetCurrentWidget();
	void SetCurrentWidget(UBJS_UserWidgetBase* currentWidget);
	class UBJS_PromptWidget* OpenPromptWidget();
	class UBJS_PromptWidget2* OpenPromptWidget2();

	UFUNCTION()
	void MoveStartPoint(AActor* Actor);

	APlayerController* GetPlayerController();
	void SetShowMouseCousor(bool Flag);

	template<class T>
	T* CustomSpawnActor(UClass* Class, FVector SpawnLocation = FVector::ZeroVector, FRotator SpawnRotation = FRotator::ZeroRotator);
	template<class T>
	void CustomDespawnActor(UClass* Class, AActor* Actor);

protected:
	UPROPERTY(VisibleAnywhere)
	class UBJS_UserWidgetBase* CurrentWidget;
	UPROPERTY(VisibleAnywhere)
	class UBJS_PromptWidget* PromptWidget;
	UPROPERTY(VisibleAnywhere)
	class UBJS_PromptWidget2* PromptWidget2;
	
	bool ShowMouseCursor;
};

template <class T>
T* ABJS_GameModeBase::CustomSpawnActor(UClass* Class, FVector SpawnLocation, FRotator SpawnRotation)
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	AActor* Actor = nullptr;

	if (TIsDerivedFrom<T, ABJS_Character>::IsDerived)
	{
		if (ActorPool.Contains(Class) && ActorPool.Find(Class)->Num() > 0)
		{
			Actor = ActorPool.Find(Class)->Pop();
			Actor->SetActorLocationAndRotation(SpawnLocation, SpawnRotation);
			Cast<ABJS_Character>(Actor)->SetActivate(true);
		}
	}
	else if (TIsDerivedFrom<T, ABJS_DemageActor>::IsDerived)
	{
		if (ActorPool.Contains(Class) && ActorPool.Find(Class)->Num() > 0)
		{
			Actor = ActorPool.Find(Class)->Pop();
			Actor->SetActorLocationAndRotation(SpawnLocation, SpawnRotation);
			Cast<ABJS_DemageActor>(Actor)->SetActivate(true);
		}
	}

	if (Actor == nullptr)
	{
		Actor = GetWorld()->SpawnActor<T>(Class, SpawnLocation, SpawnRotation, SpawnParams);
	}
	return Cast<T>(Actor);
}

template <class T>
void ABJS_GameModeBase::CustomDespawnActor(UClass* Class, AActor* Actor)
{
	if (Actor)
	{
		if (TIsDerivedFrom<T, ABJS_Character>::IsDerived)
		{
			if (!ActorPool.Contains(Class))
			{
				ActorPool.Add(Class);
			}
			ActorPool.Find(Class)->Add(Actor);
			Cast<ABJS_Character>(Actor)->SetActivate(false);
		}
		else if (TIsDerivedFrom<T, ABJS_DemageActor>::IsDerived)
		{
			if (!ActorPool.Contains(Class))
			{
				ActorPool.Add(Class);
			}
			ActorPool.Find(Class)->Add(Actor);
			UE_LOG(LogTemp, Log, TEXT("damage actor size : %d !!!"), ActorPool.Find(Class)->Num());
			Cast<ABJS_DemageActor>(Actor)->SetActivate(false);
		}
	}
} 
