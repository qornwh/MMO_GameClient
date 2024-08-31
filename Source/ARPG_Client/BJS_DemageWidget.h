// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BJS_UserWidgetBase.h"
#include "BJS_DemageWidget.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_CLIENT_API UBJS_DemageWidget : public UBJS_UserWidgetBase
{
	GENERATED_BODY()

public:
	void SetDemage(FString Demage);
	void SetCharaterDemage(FString Demage);
	void SetHeal(FString Demage);

private:
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UTextBlock* tb_demage;
};
