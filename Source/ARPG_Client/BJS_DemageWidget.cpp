// Fill out your copyright notice in the Description page of Project Settings.


#include "BJS_DemageWidget.h"

#include "Components/TextBlock.h"

void UBJS_DemageWidget::SetDemage(FString Demage)
{
	if (tb_demage)
	{
		tb_demage->SetText(FText::FromString(Demage));
		tb_demage->SetColorAndOpacity(FLinearColor(FColor::Yellow));
	}
}

void UBJS_DemageWidget::SetCharaterDemage(FString Demage)
{
	if (tb_demage)
	{
		tb_demage->SetText(FText::FromString(Demage));
		tb_demage->SetColorAndOpacity(FLinearColor(FColor::Purple));
	}
}

void UBJS_DemageWidget::SetHeal(FString Demage)
{
	if (tb_demage)
	{
		tb_demage->SetText(FText::FromString(Demage));
		tb_demage->SetColorAndOpacity(FLinearColor(FColor::Green));
	}
}
