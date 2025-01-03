// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BJS_UserWidgetBase.h"
#include "BJS_PromptWidget.generated.h"

DECLARE_DELEGATE(FOnPromptEnd)

/**
 * 
 */
UCLASS()
class ARPG_CLIENT_API UBJS_PromptWidget : public UBJS_UserWidgetBase
{
	GENERATED_BODY()

public:
	UBJS_PromptWidget();
	virtual void NativeConstruct() override;
	virtual void RemoveFromParent() override;
	void SetText(FString& text);
	void Reset();

	UFUNCTION()
	void OnClick();

	FOnPromptEnd OnPromptEnd;

private:
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UMultiLineEditableTextBox* mtb_text;
	
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UButton* btn_close;
	
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UButton* btn_ok;
};
