// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BJS_GameModeBase.h"
#include "BJS_LoginMode.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EWidgetEnum : uint8
{
	E_LOGIN = 0 UMETA(DisplayName = "LOGIN"),	
	E_MAIN = 1 UMETA(DisplayName = "MAIN"),	
	E_SELECT = 2 UMETA(DisplayName = "SELECT"),	
	E_CSTORE = 3 UMETA(DisplayName = "CHARAT_STORE"),	
	E_WSTORE = 4 UMETA(DisplayName = "WEAPON_STORE")
};

UCLASS()
class ARPG_CLIENT_API ABJS_LoginMode : public ABJS_GameModeBase
{
	GENERATED_BODY()
	
protected:
	virtual void LoadSubClass() override;
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void BeginPlay() override;

public:
	ABJS_LoginMode();
	void SetMainWidget();
	void SetSelectWidget();
	void SetCharaterStoerWidget();
	void SetWeaponStoerWidget();
	void StartGame();

protected:
	void SwitchWidget(TSubclassOf<UUserWidget> ChangeWidgetClass);

private:
	TMap<EWidgetEnum, TSubclassOf<UUserWidget>> widgetMap;

	TArray<USkeletalMesh*> CharaterMeshList;
	TArray<USkeletalMesh*> WeaponMeshList;
	TArray<UTexture2D*> CharaterImageList;
	TArray<UTexture2D*> WeaponImageList;
};
