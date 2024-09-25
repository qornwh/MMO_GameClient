// Fill out your copyright notice in the Description page of Project Settings.


#include "BJS_PromptWidget3.h"


#include "Components/Button.h"
#include "Components/EditableText.h"
#include "Components/MultiLineEditableTextBox.h"

UBJS_PromptWidget3::UBJS_PromptWidget3()
{
}

void UBJS_PromptWidget3::NativeConstruct()
{
	Super::NativeConstruct();
	
	btn_close->OnClicked.AddDynamic(this, &UBJS_PromptWidget3::OnClose);
	btn_ok->OnClicked.AddDynamic(this, &UBJS_PromptWidget3::OnClick);
}

void UBJS_PromptWidget3::SetText(FString& text)
{
	mtb_text->SetText(FText::FromString(text));
}

void UBJS_PromptWidget3::OnClick()
{
	RemoveFromParent();
	if (!tb_name->GetText().IsEmpty())
		OnSelectPromptEnd.Execute(tb_name->GetText().ToString());
}

void UBJS_PromptWidget3::OnClose()
{
	RemoveFromParent();
}

void UBJS_PromptWidget3::BJS_UpdateWidget()
{
	Super::BJS_UpdateWidget();
}
