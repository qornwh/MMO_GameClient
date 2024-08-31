// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BJS_UserWidgetBase.h"
#include "BJS_CharaterUIWidget.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_CLIENT_API UBJS_CharaterUIWidget : public UBJS_UserWidgetBase
{
	GENERATED_BODY()

public:
	UBJS_CharaterUIWidget();
	virtual void NativeConstruct() override;

	void SetName(FString Name);
	void SetHp(float Hp);
	void SetOperctiy(float Opercity);

private:
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UProgressBar* pg_hp;
	
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UTextBlock* tb_name;
};
