// Fill out your copyright notice in the Description page of Project Settings.


#include "BJS_CreateChareter_Prompt_Widget.h"

#include "Components/Button.h"
#include "Components/EditableText.h"
#include "Components/MultiLineEditableTextBox.h"

UBJS_CreateChareter_Prompt_Widget::UBJS_CreateChareter_Prompt_Widget()
{
}

void UBJS_CreateChareter_Prompt_Widget::NativeConstruct()
{
	Super::NativeConstruct();
	
	btn_close->OnClicked.AddDynamic(this, &UBJS_CreateChareter_Prompt_Widget::OnClose);
	btn_ok->OnClicked.AddDynamic(this, &UBJS_CreateChareter_Prompt_Widget::OnClick);
}

void UBJS_CreateChareter_Prompt_Widget::SetText(FString& text)
{
	mtb_text->SetText(FText::FromString(text));
}

void UBJS_CreateChareter_Prompt_Widget::OnClick()
{
	RemoveFromParent();
	if (!tb_name->GetText().IsEmpty())
		OnSelectPromptEnd.Execute(tb_name->GetText().ToString());
}

void UBJS_CreateChareter_Prompt_Widget::OnClose()
{
	RemoveFromParent();
}

void UBJS_CreateChareter_Prompt_Widget::BJS_UpdateWidget()
{
	Super::BJS_UpdateWidget();
}
