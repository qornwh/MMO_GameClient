// Fill out your copyright notice in the Description page of Project Settings.


#include "BJS_ItemSlotWidget.h"

#include "Components/CheckBox.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

UBJS_ItemSlotWidget::UBJS_ItemSlotWidget()
{
}

void UBJS_ItemSlotWidget::BJS_InitWidget()
{
	Super::BJS_InitWidget();
}

void UBJS_ItemSlotWidget::BJS_SubUpdateWidget()
{
	Super::BJS_SubUpdateWidget();
}

void UBJS_ItemSlotWidget::SetEquip(EquipItem& Item)
{
	CurEquipItem = Item;
}

void UBJS_ItemSlotWidget::SetEtc(EtcItem& Item)
{
	CurEtcItem = Item;
}

EquipItem& UBJS_ItemSlotWidget::GetEquip()
{
	return CurEquipItem;
}

EtcItem& UBJS_ItemSlotWidget::GetEtc()
{
	return CurEtcItem;
}

void UBJS_ItemSlotWidget::SetImg(UTexture2D* Image)
{
	if (!Image)
	{
		SetSlots(false);
		return;
	}
	SetSlots(true);
	FSlateBrush Brush;
	Brush.SetResourceObject(Image);
	Brush.ImageSize = FVector2D(Image->GetSizeX(), Image->GetSizeY());
	img_item->SetBrush(Brush);
}

void UBJS_ItemSlotWidget::SetCnt(int32 Cnt)
{
	if (Cnt > 1)
	{
		FString str = FString::FromInt(Cnt);
		tb_cnt->SetText(FText::FromString(str));
	}
	else
	{
		tb_cnt->SetText(FText());
	}
}

void UBJS_ItemSlotWidget::SetSlots(bool Flag)
{
	if (Flag)
	{
		img_item->SetVisibility(ESlateVisibility::Visible);
		tb_cnt->SetVisibility(ESlateVisibility::Visible);
		cb_check->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		img_item->SetVisibility(ESlateVisibility::Hidden);
		tb_cnt->SetVisibility(ESlateVisibility::Hidden);
		cb_check->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UBJS_ItemSlotWidget::ResetCheck()
{
	cb_check->SetCheckedState(ECheckBoxState::Unchecked);
}
