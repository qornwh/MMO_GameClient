// Fill out your copyright notice in the Description page of Project Settings.


#include "BJS_HeaderWidget.h"

#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/TextBlock.h"

UBJS_HeaderWidget::UBJS_HeaderWidget()
{
}

UBJS_HeaderWidget::~UBJS_HeaderWidget()
{
}

void UBJS_HeaderWidget::OnBackEvent()
{
	OnBack.Execute();
}

void UBJS_HeaderWidget::BJS_InitWidget()
{
	Super::BJS_InitWidget();
}

void UBJS_HeaderWidget::BJS_SubUpdateWidget()
{
	Super::BJS_SubUpdateWidget();
}

void UBJS_HeaderWidget::SetTextTitle(FString Title)
{
	tb_title->SetText(FText::FromString(Title));
}

void UBJS_HeaderWidget::SetTextGold(int32 Gold)
{
	FString str = FString::FromInt(Gold);
	tb_gold->SetText(FText::FromString(str));
	if (cp_gold->GetVisibility() == ESlateVisibility::Hidden)
		cp_gold->SetVisibility(ESlateVisibility::Visible);
}

void UBJS_HeaderWidget::SetTextCash(int32 Cash)
{
	FString str = FString::FromInt(Cash);
	tb_cash->SetText(FText::FromString(str));
	if (cp_cash->GetVisibility() == ESlateVisibility::Hidden)
		cp_cash->SetVisibility(ESlateVisibility::Visible);
}

void UBJS_HeaderWidget::NativeConstruct()
{
	Super::NativeConstruct();

	btn_back->OnClicked.AddDynamic(this, &UBJS_HeaderWidget::OnBackEvent);

	cp_gold->SetVisibility(ESlateVisibility::Hidden);
	cp_cash->SetVisibility(ESlateVisibility::Hidden);
}
