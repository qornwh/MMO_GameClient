// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BJS_Character.h"
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

	template<class T>
	T* CustomSpawnActor(UClass* Class, FVector SpawnLocation = FVector::ZeroVector, FRotator SpawnRotation = FRotator::ZeroRotator);
	template<class T>
	void CustomDespawnActor(UClass* Class, AActor* Actor);

// private:
	// 일단 오브젝트 풀링을 사용한다.
	TMap<UClass*, TArray<TObjectPtr<AActor>>> CharaterPool;

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
		if (CharaterPool.Contains(Class) && CharaterPool.Find(Class)->Num() > 0)
		{
			Actor = CharaterPool.Find(Class)->Pop();
			Cast<ABJS_Character>(Actor)->SetActivate(true);
			Actor->SetActorLocationAndRotation(SpawnLocation, SpawnRotation);
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
			if (!CharaterPool.Contains(Class))
			{
				CharaterPool.Add(Class);
			}
			CharaterPool.Find(Class)->Add(Actor);
			Cast<ABJS_Character>(Actor)->SetActivate(false);
		}
	}
} 
