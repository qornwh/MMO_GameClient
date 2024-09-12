// Fill out your copyright notice in the Description page of Project Settings.


#include "BJS_StateWidget.h"

#include "Components/TextBlock.h"

void UBJS_StateWidget::BJS_SubUpdateWidget()
{
	Super::BJS_SubUpdateWidget();
}

void UBJS_StateWidget::SetName(FString Name)
{
	tb_name->SetText(FText::FromString(Name));
}

void UBJS_StateWidget::SetLv(int32 Lv)
{
	FString str = FString::FromInt(Lv);
	tb_lv->SetText(FText::FromString(str));
}

void UBJS_StateWidget::SetAttack(int32 Attack)
{
	FString str = FString::FromInt(Attack);
	tb_attack->SetText(FText::FromString(str));
}

void UBJS_StateWidget::SetSpeed(int32 Speed)
{
	FString str = FString::FromInt(Speed);
	tb_speed->SetText(FText::FromString(str));
}
