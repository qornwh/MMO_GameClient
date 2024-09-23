// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BJS_UserWidgetBase.h"
#include "BJS_PromptWidget2.generated.h"

DECLARE_DELEGATE(FOnPromptOk)
DECLARE_DELEGATE(FOnPromptCancle)

/**
 * 
 */
UCLASS()
class ARPG_CLIENT_API UBJS_PromptWidget2 : public UBJS_UserWidgetBase
{
	GENERATED_BODY()

public:
	UBJS_PromptWidget2();
	virtual void NativeConstruct() override;
	void SetText(FString& text);
	void Reset();

	UFUNCTION()
	void OnOk();
	UFUNCTION()
	void OnCancle();
	virtual void RemoveFromParent() override;

	FOnPromptOk OnPromptOk;
	FOnPromptCancle OnPromptCancle;

private:
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UMultiLineEditableTextBox* mtb_text;
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UButton* btn_close;
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UButton* btn_ok;
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UButton* btn_cancle;
};
