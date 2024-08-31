// Fill out your copyright notice in the Description page of Project Settings.


#include "BJS_ItemSlotWidget.h"

#include "Components/CheckBox.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

UBJS_ItemSlotWidget::UBJS_ItemSlotWidget() :EquipItem(-1), EtcItem(-1, 0)
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

void UBJS_ItemSlotWidget::SetEquip(int32 Code)
{
	EtcItem.Code = -1;
	EtcItem.Count = 0;

	EquipItem.Code = Code;
}

void UBJS_ItemSlotWidget::SetEquip(int32 Code, int32 Count)
{
	EquipItem.Code = -1;
	
	EtcItem.Code = Code;
	EtcItem.Count = Count;
}

EquipItem& UBJS_ItemSlotWidget::GetEquip()
{
	return EquipItem;
}

EtcItem& UBJS_ItemSlotWidget::GetEtc()
{
	return EtcItem;
}

void UBJS_ItemSlotWidget::SetImg(UTexture2D* Image)
{
	if (!Image)
	{
		ShowImg(false);
	}
	ShowImg(true);
	FSlateBrush Brush;
	Brush.SetResourceObject(Image);
	Brush.ImageSize = FVector2D(Image->GetSizeX(), Image->GetSizeY());
	img_item->SetBrush(Brush);
}

void UBJS_ItemSlotWidget::SetCnt(int32 Cnt)
{
	if (Cnt > 0)
	{
		FString str = FString::FromInt(Cnt);
		tb_cnt->SetText(FText::FromString(str));
	}
	else
	{
		tb_cnt->SetText(FText());
	}
}

void UBJS_ItemSlotWidget::ShowImg(bool Flag)
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

void UBJS_ItemSlotWidget::OnMouseClickHandler(FGeometry MyGeometry, const FPointerEvent& MouseEvent)
{
	// 부모 바인딩
}

FReply UBJS_ItemSlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    OnMouseClickHandler(InGeometry, InMouseEvent);
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}
