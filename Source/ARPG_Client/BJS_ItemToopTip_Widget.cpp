// Fill out your copyright notice in the Description page of Project Settings.


#include "BJS_ItemToopTip_Widget.h"

#include "BJS_GameInstance.h"
#include "ItemEquipStruct.h"
#include "ItemEtcStruct.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/MultiLineEditableTextBox.h"
#include "Components/TextBlock.h"

UBJS_ItemToopTip_Widget::UBJS_ItemToopTip_Widget()
{
}

void UBJS_ItemToopTip_Widget::NativeConstruct()
{
	Super::NativeConstruct();

	if (btn_close)
	{
		btn_close->OnClicked.AddDynamic(this, &UBJS_ItemToopTip_Widget::OnClose);
	}
}

void UBJS_ItemToopTip_Widget::OnClose()
{
	SetVisibility(ESlateVisibility::Hidden);
}

void UBJS_ItemToopTip_Widget::SetEquipItem(EquipItem& NewEquipItem)
{
	auto instance = Cast<UBJS_GameInstance>(GetGameInstance());
	if (instance)
	{
		CurEquipItem = NewEquipItem;
		int32 Code = CurEquipItem.ItemCode;
		if (Code < 0)
			return;
		auto& ImageMap = instance->GetItemEquipIconImgMap();
		if (ImageMap.Contains(Code))
		{
			SetImg(ImageMap[Code]);
		}
		auto& EquipMap = instance->GetItemEquipStructMap();
		if (EquipMap.Contains(Code))
		{
			SetName(EquipMap[Code]->Name.ToString());
			SetType(EquipMap[Code]->Type);
			SetDescription(EquipMap[Code]->Description.ToString());
		}

		SetAttack(CurEquipItem.Attack);
		SetSpeed(CurEquipItem.Speed);
	}
}

void UBJS_ItemToopTip_Widget::SetEtcItem(EtcItem& NewEtcItem)
{
	auto instance = Cast<UBJS_GameInstance>(GetGameInstance());
	if (instance)
	{
		CurEtcItem = NewEtcItem;
		int32 Code = CurEtcItem.ItemCode;
		if (Code < 0)
			return;
		auto& ImageMap = instance->GetItemEtcIconImgMap();
		if (ImageMap.Contains(Code))
		{
			SetImg(ImageMap[Code]);
		}
		auto& EtcMap = instance->GetItemEtcStructMap();
		if (EtcMap.Contains(Code))
		{
			SetName(EtcMap[Code]->Name.ToString());
			SetType(EtcMap[Code]->Type);
			SetDescription(EtcMap[Code]->Description.ToString());
		}

		SetAttack(-1);
		SetSpeed(-1);
	}
}

void UBJS_ItemToopTip_Widget::ResetInfo()
{
	type = ToolTipType::NOT_DATA;
	CurEquipItem.SetEmptyItem();
	CurEtcItem.SetEmptyItem();
}

void UBJS_ItemToopTip_Widget::SetImg(UTexture2D* Image)
{
	if (!Image)
	{
		return;
	}
	FSlateBrush Brush;
	Brush.SetResourceObject(Image);
	Brush.ImageSize = FVector2D(Image->GetSizeX(), Image->GetSizeY());
	img_icon->SetBrush(Brush);
}

void UBJS_ItemToopTip_Widget::SetName(FString Name)
{
	tb_name->SetText(FText::FromString(Name));
}

void UBJS_ItemToopTip_Widget::SetType(int32 Type)
{
	FString str = FString::FromInt(Type);
	tb_type->SetText(FText::FromString(str));
}

void UBJS_ItemToopTip_Widget::SetAttack(int32 Attack)
{
	if (Attack < 0)
	{
		n_tb_attack->SetVisibility(ESlateVisibility::Hidden);
		tb_attack->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		n_tb_attack->SetVisibility(ESlateVisibility::Visible);
		tb_attack->SetVisibility(ESlateVisibility::Visible);
		FString str = FString::FromInt(Attack);
		tb_attack->SetText(FText::FromString(str));
	}
}

void UBJS_ItemToopTip_Widget::SetSpeed(int32 Speed)
{
	if (Speed < 0)
	{
		n_tb_speed->SetVisibility(ESlateVisibility::Hidden);
		tb_speed->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		n_tb_speed->SetVisibility(ESlateVisibility::Visible);
		tb_speed->SetVisibility(ESlateVisibility::Visible);
		FString str = FString::FromInt(Speed);
		tb_speed->SetText(FText::FromString(str));
	}
}

void UBJS_ItemToopTip_Widget::SetDescription(FString Description)
{
	mtb_description->SetText(FText::FromString(Description));
}
