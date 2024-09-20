// Fill out your copyright notice in the Description page of Project Settings.


#include "BJS_MailSlot.h"

#include "BJS_MailWidget.h"
#include "Components/Button.h"
#include "Components/CheckBox.h"
#include "Components/TextBlock.h"

UBJS_MailSlot::UBJS_MailSlot()
{
}

UBJS_MailSlot::~UBJS_MailSlot()
{
}

void UBJS_MailSlot::SetSlot(bool Flag)
{
	if (Flag)
	{
		btn_select->SetIsEnabled(true);
		tb_title->SetVisibility(ESlateVisibility::Visible);
		cb_check->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		btn_select->SetIsEnabled(false);
		tb_title->SetVisibility(ESlateVisibility::Hidden);
		cb_check->SetVisibility(ESlateVisibility::Hidden);
		CurrentMail.SetEmptyItem();
	}
}

void UBJS_MailSlot::SetParentWidget(UBJS_MailWidget* Widget)
{
	parentWidget = Widget;
	btn_select->OnClicked.AddDynamic(this, &UBJS_MailSlot::SelectMail);
}

void UBJS_MailSlot::SetMail(Mail& NewMail)
{
	CurrentMail = NewMail;

	tb_title->SetText(FText::FromString(NewMail.Title));
	cb_check->SetCheckedState(ECheckBoxState::Unchecked);
	// TODO : read, not read 체크 
}

void UBJS_MailSlot::SelectMail()
{
	if (parentWidget)
	{
		parentWidget->SetMailInfo(CurrentMail.Code);
	}
}

void UBJS_MailSlot::NativeConstruct()
{
	Super::NativeConstruct();
}
