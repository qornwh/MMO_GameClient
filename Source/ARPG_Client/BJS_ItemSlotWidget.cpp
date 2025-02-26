// Fill out your copyright notice in the Description page of Project Settings.


#include "BJS_ItemSlotWidget.h"

#include "BJS_InGameMode.h"
#include "CustomButton.h"
#include "ItemDragDropOperation.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/CheckBox.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

UBJS_ItemSlotWidget::UBJS_ItemSlotWidget()
{
}

void UBJS_ItemSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (!OnDoubleClick.IsBound())
		OnDoubleClick.AddDynamic(this, &UBJS_ItemSlotWidget::SendToolTipItemInfo);
	if (!OnRightClick.IsBound())
		OnRightClick.AddDynamic(this, &UBJS_ItemSlotWidget::OnSendItem);
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
	SetCnt(Item.Count);
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
	}
	else
	{
		img_item->SetVisibility(ESlateVisibility::Hidden);
		tb_cnt->SetVisibility(ESlateVisibility::Hidden);
		cb_check->SetVisibility(ESlateVisibility::Hidden);
		CurEquipItem.SetEmptyItem();
		CurEtcItem.SetEmptyItem();
	}
}

void UBJS_ItemSlotWidget::SetSocket(bool Flag)
{
	if (Flag)
	{
		img_item->SetVisibility(ESlateVisibility::Visible);
		tb_cnt->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		img_item->SetVisibility(ESlateVisibility::Hidden);
		tb_cnt->SetVisibility(ESlateVisibility::Hidden);
		CurEquipItem.SetEmptyItem();
		CurEtcItem.SetEmptyItem();
	}
	cb_check->SetVisibility(ESlateVisibility::Hidden);
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
	
	img_item->SetVisibility(ESlateVisibility::Visible);
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

void UBJS_ItemSlotWidget::SetSocketType(ITEMSOCKETTYPE SocketType)
{
	CurSocketType = SocketType;
}

void UBJS_ItemSlotWidget::SetSocketPosition(int32 Position)
{
	CurSocketPosition = Position;
}

void UBJS_ItemSlotWidget::OnSendItem()
{
	if (SendItem.IsBound())
	{
		if (CurEquipItem.InvenPos >= 0)
		{
			if (CurEquipItem.EquipType == EquipItemType::AttackItem)
				SendItem.Execute(CurEquipItem.InvenPos, 1);
			else if (CurEquipItem.EquipType == EquipItemType::SpeedItem)
				SendItem.Execute(CurEquipItem.InvenPos, 2);
		}
		else
		{
			SendItem.Execute(-1, CurEquipItem.EquipPos);
		}
	}
}

void UBJS_ItemSlotWidget::SendToolTipItemInfo()
{
	auto mode = Cast<ABJS_InGameMode>(GetWorld()->GetAuthGameMode());
	if (mode)
	{
		if (Type == ToolTipType::EQUIP_ITEM_TYPE)
		{
			if (!CurEquipItem.IsEmpty())
			{
				mode->UpdateToolTipEquipItem(CurEquipItem);
				mode->OpenToolTipUI();
			}
		}
		else if (Type == ToolTipType::ETC_ITEM_TYPE)
		{
			if (!CurEtcItem.IsEmpty())
			{
				mode->UpdateToolTipEtcItem(CurEtcItem);
				mode->OpenToolTipUI();
			}
		}
	}
}

bool UBJS_ItemSlotWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	auto mode = Cast<ABJS_InGameMode>(GetWorld()->GetAuthGameMode());
	if (mode)
	{
		UItemDragDropOperation* Operation = Cast<UItemDragDropOperation>(InOperation);
		if (Operation && Operation->GetWidgetReference())
		{
			// 드래그앤드롭 성공 시 수행할 동작
			auto Target = Cast<UBJS_ItemSlotWidget>(Operation->GetWidgetReference());
			if (Target && !Target->GetEquip().IsEmpty())
			{
				if (CurSocketType == ITEMSOCKETTYPE::SENDMAIL)
				{
					mode->SetMailEquipItem(Target->GetEquip().InvenPos, CurSocketPosition);
				}
			}
		}
	}
	return false;
}

FReply UBJS_ItemSlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (CurSocketType == ITEMSOCKETTYPE::SENDMAIL)
	{
		if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
		{
			SetSocket(false);
		}
		return FReply::Handled();
	}

	if (CurSocketType == ITEMSOCKETTYPE::SUBINVENTORY)
	{
		if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
		{
			// 왼쪽 클릭 드래그 앤 드롭 시작
			UDragDropOperation* DragDropOperation = NewObject<UDragDropOperation>();
			DragDropOperation->DefaultDragVisual = this;
			return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;
		}
	}
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}
