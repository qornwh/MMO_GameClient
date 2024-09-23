// Fill out your copyright notice in the Description page of Project Settings.


#include "BJS_PromptWidget2.h"

#include "Components/Button.h"
#include "Components/MultiLineEditableTextBox.h"

UBJS_PromptWidget2::UBJS_PromptWidget2()
{
}

void UBJS_PromptWidget2::NativeConstruct()
{
	Super::NativeConstruct();
	if (!btn_close->OnClicked.IsBound())
		btn_close->OnClicked.AddDynamic(this, &UBJS_PromptWidget2::OnCancle);
	if (!btn_ok->OnClicked.IsBound())
		btn_ok->OnClicked.AddDynamic(this, &UBJS_PromptWidget2::OnOk);
	if (!btn_cancle->OnClicked.IsBound())
		btn_cancle->OnClicked.AddDynamic(this, &UBJS_PromptWidget2::OnCancle);
}

void UBJS_PromptWidget2::SetText(FString& text)
{
	mtb_text->SetText(FText::FromString(text));
}

void UBJS_PromptWidget2::Reset()
{
	if (mtb_text)
		mtb_text->SetText(FText{});
	if(OnPromptOk.IsBound())
		OnPromptOk.Unbind();
	if (OnPromptCancle.IsBound())
		OnPromptCancle.Unbind();
}

void UBJS_PromptWidget2::OnOk()
{
	UE_LOG(LogTemp, Log, TEXT("OnClick PROMPT !!!"));
	OnPromptOk.Execute();
	RemoveFromParent();
}

void UBJS_PromptWidget2::OnCancle()
{
	OnPromptCancle.Execute();
	RemoveFromParent();
}

void UBJS_PromptWidget2::RemoveFromParent()
{
	Super::RemoveFromParent();
	Reset();
}
