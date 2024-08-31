// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BJS_SubWidget.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_CLIENT_API UBJS_SubWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void BJS_InitWidget();
	virtual void BJS_SubUpdateWidget();
};
