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

	if (!btn_item->OnDoubleClick.IsBound())
		btn_item->OnDoubleClick.AddDynamic(this, &UBJS_ItemSlotWidget::SendToolTipItemInfo);
}

void UBJS_ItemSlotWidget::SetEquip(EquipItem& Item)
{
	CurEquipItem = Item;
	CurEtcItem.SetEmptyItem();
	Type = ToolTipType::EQUIP_ITEM_TYPE;
}

void UBJS_ItemSlotWidget::SetEtc(EtcItem& Item)
{
	CurEtcItem = Item;
	CurEquipItem.SetEmptyItem();
	Type = ToolTipType::ETC_ITEM_TYPE;
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
		img_item->SetVisibility(ESlateVisibility::Hidden);
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
		
		if (!btn_item->OnRightClick.IsBound())
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

void UBJS_ItemSlotWidget::SetSocket(bool Flag)
{
	if (Flag)
	{
		img_item->SetVisibility(ESlateVisibility::Visible);
		
		if (!btn_item->OnRightClick.IsBound())
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

void UBJS_ItemSlotWidget::SetRecive(bool Flag)
{
	if (Flag)
	{
		img_item->SetOpacity(1);
	}
	else
	{
		img_item->SetOpacity(0.5);
	}
	
	cb_check->SetVisibility(ESlateVisibility::Hidden);
	if (Type == ToolTipType::EQUIP_ITEM_TYPE)
	{
		tb_cnt->SetVisibility(ESlateVisibility::Hidden);
	}
	else if (Type == ToolTipType::ETC_ITEM_TYPE)
	{
		tb_cnt->SetVisibility(ESlateVisibility::Visible);
		SetCnt(CurEtcItem.Count);
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

void UBJS_ItemSlotWidget::SendToolTipItemInfo()
{
	auto mode = Cast<ABJS_InGameMode>(GetWorld()->GetAuthGameMode());
	if (mode)
	{
		if (Type == ToolTipType::EQUIP_ITEM_TYPE)
			mode->UpdateToolTipEquipItem(CurEquipItem);
		else if (Type == ToolTipType::ETC_ITEM_TYPE)
			mode->UpdateToolTipEtcItem(CurEtcItem);
		mode->OpenToolTipUI();
	}
}
