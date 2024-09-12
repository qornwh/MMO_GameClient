// Fill out your copyright notice in the Description page of Project Settings.


#include "BJS_ItemSlotWidget.h"

#include "BJS_InGameMode.h"
#include "CustomButton.h"
#include "Components/CheckBox.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

UBJS_ItemSlotWidget::UBJS_ItemSlotWidget()
{
}

void UBJS_ItemSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();
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
		btn_item->OnRightClick.AddDynamic(this, &UBJS_ItemSlotWidget::SendItemEquipped);
	}
	else
	{
		img_item->SetVisibility(ESlateVisibility::Hidden);
		tb_cnt->SetVisibility(ESlateVisibility::Hidden);
		cb_check->SetVisibility(ESlateVisibility::Hidden);
		btn_item->OnRightClick.RemoveAll(this);
	}
}

void UBJS_ItemSlotWidget::ResetCheck()
{
	cb_check->SetCheckedState(ECheckBoxState::Unchecked);
}

void UBJS_ItemSlotWidget::SetEmptyEquip()
{
	CurEquipItem.SetEmptyItem();
}

void UBJS_ItemSlotWidget::SetEtcEquip()
{
	CurEtcItem.SetEmptyItem();
}

void UBJS_ItemSlotWidget::SetSocket(bool Flag)
{
	if (Flag)
	{
		img_item->SetVisibility(ESlateVisibility::Visible);
		btn_item->OnRightClick.AddDynamic(this, &UBJS_ItemSlotWidget::SendItemEquipped);
	}
	else
	{
		img_item->SetVisibility(ESlateVisibility::Hidden);
		btn_item->OnRightClick.RemoveAll(this);
	}
	cb_check->SetVisibility(ESlateVisibility::Hidden);
	tb_cnt->SetVisibility(ESlateVisibility::Hidden);
}

void UBJS_ItemSlotWidget::SendItemEquipped()
{
	auto mode = Cast<ABJS_InGameMode>(GetWorld()->GetAuthGameMode());
	if (mode)
	{
		if (CurEquipItem.UniqueId > 0)
		{
			mode->SendEquippedItem(CurEquipItem);
		}
	}
}
