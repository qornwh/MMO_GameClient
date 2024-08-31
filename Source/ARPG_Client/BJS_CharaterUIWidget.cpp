// Fill out your copyright notice in the Description page of Project Settings.


#include "BJS_CharaterUIWidget.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

UBJS_CharaterUIWidget::UBJS_CharaterUIWidget()
{
}

void UBJS_CharaterUIWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UBJS_CharaterUIWidget::SetName(FString Name)
{
	tb_name->SetText(FText::FromString(Name));
}

void UBJS_CharaterUIWidget::SetHp(float Hp)
{
	pg_hp->SetPercent(Hp);
}

void UBJS_CharaterUIWidget::SetOperctiy(float Opercity)
{
	tb_name->SetOpacity(Opercity);
	pg_hp->SetRenderOpacity(Opercity);
}
