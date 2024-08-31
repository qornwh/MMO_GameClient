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
	btn_close->OnClicked.AddDynamic(this, &UBJS_PromptWidget2::OnCancle);
	btn_ok->OnClicked.AddDynamic(this, &UBJS_PromptWidget2::OnOk);
	btn_cancle->OnClicked.AddDynamic(this, &UBJS_PromptWidget2::OnCancle);
}

void UBJS_PromptWidget2::SetText(FString& text)
{
	mtb_text->SetText(FText::FromString(text));
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

void UBJS_PromptWidget2::BJS_UpdateWidget()
{
	Super::BJS_UpdateWidget();
}
