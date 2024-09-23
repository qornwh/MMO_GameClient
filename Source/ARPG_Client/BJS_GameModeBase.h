// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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

	template< class T >
	T* CustomSpawnActor(UClass* Class, FVector SpawnLocation = FVector::ZeroVector, FRotator SpawnRotation = FRotator::ZeroRotator);

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
	return GetWorld()->SpawnActor<T>(Class, SpawnLocation, SpawnRotation, SpawnParams);
}
