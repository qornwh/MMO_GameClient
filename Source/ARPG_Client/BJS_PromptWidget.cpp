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
	if (!btn_close->OnClicked.IsBound())
		btn_close->OnClicked.AddDynamic(this, &UBJS_PromptWidget::OnClick);
	if (!btn_ok->OnClicked.IsBound())
		btn_ok->OnClicked.AddDynamic(this, &UBJS_PromptWidget::OnClick);
}

void UBJS_PromptWidget::SetText(FString& text)
{
	mtb_text->SetText(FText::FromString(text));
}

void UBJS_PromptWidget::Reset()
{
	if (mtb_text)
		mtb_text->SetText(FText{});
	if (OnPromptEnd.IsBound())
		OnPromptEnd.Unbind();
}

void UBJS_PromptWidget::RemoveFromParent()
{
	Super::RemoveFromParent();
	Reset();
}

void UBJS_PromptWidget::OnClick()
{
	if (OnPromptEnd.IsBound())
		OnPromptEnd.Execute();
	RemoveFromParent();
}
