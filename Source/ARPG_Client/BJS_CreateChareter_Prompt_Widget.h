// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BJS_UserWidgetBase.h"
#include "BJS_CreateChareter_Prompt_Widget.generated.h"

DECLARE_DELEGATE_OneParam(FOnSelectPromptEnd, FString)

/**
 * 
 */
UCLASS()
class ARPG_CLIENT_API UBJS_CreateChareter_Prompt_Widget : public UBJS_UserWidgetBase
{
	GENERATED_BODY()
	
public:
	UBJS_CreateChareter_Prompt_Widget();
	virtual void NativeConstruct() override;
	void SetText(FString& text);

	UFUNCTION()
	void OnClick();

	UFUNCTION()
	void OnClose();
	
	FOnSelectPromptEnd OnSelectPromptEnd;

	virtual void BJS_UpdateWidget() override;

private:
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UMultiLineEditableTextBox* mtb_text;
	
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UEditableText* tb_name;
	
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UButton* btn_close;
	
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UButton* btn_ok;
};