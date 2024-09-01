// Fill out your copyright notice in the Description page of Project Settings.


#include "BJS_PromptWidget.h"

#include "Components/Button.h"
#include "Components/MultiLineEditableTextBox.h"

UBJS_PromptWidget::UBJS_PromptWidget()
{
}

void UBJS_PromptWidget::NativeConstruct()
{
	Super::NativeConstruct();
	btn_close->OnClicked.AddDynamic(this, &UBJS_PromptWidget::OnClick);
	btn_ok->OnClicked.AddDynamic(this, &UBJS_PromptWidget::OnClick);
}

void UBJS_PromptWidget::SetText(FString& text)
{
	mtb_text->SetText(FText::FromString(text));
}

void UBJS_PromptWidget::OnClick()
{
	RemoveFromParent();
	if (OnPromptEnd.IsBound())
		OnPromptEnd.Execute();
}

void UBJS_PromptWidget::BJS_UpdateWidget()
{
	Super::BJS_UpdateWidget();
}
