// Fill out your copyright notice in the Description page of Project Settings.


#include "BJS_InventoryWidget.h"

#include "BJS_CharaterState.h"
#include "BJS_GameInstance.h"
#include "BJS_InGameMode.h"
#include "BJS_ItemSlotWidget.h"
#include "BJS_PromptWidget2.h"
#include "BJS_StateWidget.h"
#include "InventoryItem.h"
#include "Components/Button.h"
#include "Components/GridPanel.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"

UBJS_InventoryWidget::UBJS_InventoryWidget()
{
	static ConstructorHelpers::FClassFinder<UUserWidget> ITEM_SLOT_HUD(
		TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/MyGame/UMG/BJS_SWBP_ItemSlot.BJS_SWBP_ItemSlot_C'"));
	check(ITEM_SLOT_HUD.Class);

	ItemSlotClass = ITEM_SLOT_HUD.Class;
	InventoryModeState = InventoryMode::EQUIP;
}

void UBJS_InventoryWidget::BJS_InitWidget()
{
	Super::BJS_InitWidget();

	auto instance = Cast<UBJS_GameInstance>(GetGameInstance());
	if (instance)
	{
		int32 Code = instance->GetCurCharaterType();
		if (instance->GetCharaterImgMap().Contains(Code))
		{
			auto Image = instance->GetCharaterImgMap()[Code];
			if (Image)
			{
				FSlateBrush Brush;
				Brush.SetResourceObject(Image);
				Brush.ImageSize = FVector2D(Image->GetSizeX(), Image->GetSizeY());
				img_charater->SetBrush(Brush);
			}
		}
	}

	for (int32 i = 0; i < RowSize; i++)
	{
		for (int32 j = 0; j < ColSize; j++)
		{
			auto equipSlot = CreateWidget<UBJS_ItemSlotWidget>(GetWorld(), ItemSlotClass);
			auto etcSlot = CreateWidget<UBJS_ItemSlotWidget>(GetWorld(), ItemSlotClass);
			equipSlot->ShowImg(false);
			etcSlot->ShowImg(false);
			ConstSlot.Add(equipSlot);
			ConstSlot.Add(etcSlot);
			EmptyEquipSlot.Add(equipSlot);
			EmptyEtcSlot.Add(etcSlot);
		}
	}

	btn_equip->OnClicked.AddDynamic(this, &UBJS_InventoryWidget::ViewInventoryEquip);
	btn_etc->OnClicked.AddDynamic(this, &UBJS_InventoryWidget::ViewInventoryEtc);
	btn_sell->OnClicked.AddDynamic(this, &UBJS_InventoryWidget::SellItem);
	btn_check->OnClicked.AddDynamic(this, &UBJS_InventoryWidget::InvetoryReset);
	SetSlot();
	ViewInventoryEquip();
}

void UBJS_InventoryWidget::BJS_UpdateWidget()
{
	Super::BJS_UpdateWidget();

	auto instance = Cast<UBJS_GameInstance>(GetGameInstance());
	if (instance)
	{
		wg_state->SetAttack(instance->GetMyState()->GetAttack());
		wg_state->SetSpeed(instance->GetMyState()->GetSpeed());

		SetGold(instance->GetMyInventory()->GetGold());
	}
}

void UBJS_InventoryWidget::AddEquipSlot(int32 EquipUnipeId)
{
	if (!EmptyEquipSlot.IsEmpty())
	{
		auto instance = Cast<UBJS_GameInstance>(GetGameInstance());
		if (instance)
		{
			if (instance->GetMyInventory()->GetEquipItems().Contains(EquipUnipeId))
			{
				auto slot = EmptyEquipSlot.Last();
				EmptyEquipSlot.Pop();
				auto& ImageMap = instance->GetItemEquipIconImgMap();
				auto item = instance->GetMyInventory()->GetEquipItems().Find(EquipUnipeId);
				slot->SetEquip(*item);
				slot->SetImg(ImageMap[item->ItemCode]);
				EquipItemSlots.Add(EquipUnipeId, slot);
			}
		}
	}
}

void UBJS_InventoryWidget::RemoveEquipSlot(int32 EquipUnipeId)
{
	auto instance = Cast<UBJS_GameInstance>(GetGameInstance());
	if (instance)
	{
		if (!instance->GetMyInventory()->GetEquipItems().Contains(EquipUnipeId))
		{
			auto slot = EquipItemSlots.Find(EquipUnipeId);
			EquipItemSlots.Remove(EquipUnipeId);
			(*slot)->ShowImg(false);
			EmptyEquipSlot.Add(*slot);
		}
	}
}

void UBJS_InventoryWidget::UpdateEtcSlot(int32 EtcItemCode)
{
	auto instance = Cast<UBJS_GameInstance>(GetGameInstance());
	if (instance)
	{
		if (instance->GetMyInventory()->GetEtcItems().Contains(EtcItemCode))
		{
			// 추가, 업데이트
			if (EtcItemSlots.Contains(EtcItemCode))
			{
				auto slot = EtcItemSlots[EtcItemCode];
				auto item = instance->GetMyInventory()->GetEtcItems().Find(EtcItemCode);
				slot->SetEtc(*item);
				slot->SetCnt(item->Count);
			}
			else
			{
				auto slot = EmptyEtcSlot.Last();
				EmptyEtcSlot.Pop();
				auto& ImageMap = instance->GetItemEtcIconImgMap();
				auto item = instance->GetMyInventory()->GetEtcItems().Find(EtcItemCode);
				slot->SetEtc(*item);
				slot->SetImg(ImageMap[item->ItemCode]);
				slot->SetCnt(item->Count);
				EtcItemSlots.Add(EtcItemCode, slot);
			}
		}
		else
		{
			// 삭제
			auto slot = EtcItemSlots.Find(EtcItemCode);
			EtcItemSlots.Remove(EtcItemCode);
			(*slot)->ShowImg(false);
			EmptyEtcSlot.Add(*slot);
		}
	}
}

void UBJS_InventoryWidget::SetSlot()
{
	// 인벤토리 업데이트 될때만 처리
	SlotResetCheck();

	int32 row = 0;
	int32 col = 0;
	if ((InventoryModeState & NOTSELL) == InventoryMode::EQUIP)
	{
		ugp_EquipItemSlots->ClearChildren();
		for (auto equipItemEntry : EquipItemSlots)
		{
			UUniformGridSlot* GridSlot = ugp_EquipItemSlots->AddChildToUniformGrid(equipItemEntry.Value, row, col);
			col++;
			if (col >= ColSize)
			{
				col = 0;
				row++;
			}
			if (GridSlot)
			{
				GridSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
				GridSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);
			}
		}

		for (auto empty : EmptyEquipSlot)
		{
			UUniformGridSlot* GridSlot = ugp_EquipItemSlots->AddChildToUniformGrid(empty, row, col);
			col++;
			if (col >= ColSize)
			{
				col = 0;
				row++;
			}
			if (GridSlot)
			{
				GridSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
				GridSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);
			}
		}
	}
	else
	{
		ugp_ItemSlots->ClearChildren();
		for (auto etcItemEntry : EtcItemSlots)
		{
			UUniformGridSlot* GridSlot = ugp_ItemSlots->AddChildToUniformGrid(etcItemEntry.Value, row, col);
			col++;
			if (col >= ColSize)
			{
				col = 0;
				row++;
			}
			if (GridSlot)
			{
				GridSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
				GridSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);
			}
		}

		for (auto empty : EmptyEtcSlot)
		{
			UUniformGridSlot* GridSlot = ugp_ItemSlots->AddChildToUniformGrid(empty, row, col);
			col++;
			if (col >= ColSize)
			{
				col = 0;
				row++;
			}
			if (GridSlot)
			{
				GridSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
				GridSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);
			}
		}
	}
}

void UBJS_InventoryWidget::SlotResetCheck()
{
	for (auto equipItemEntry : EquipItemSlots)
	{
		if (equipItemEntry.Value)
			equipItemEntry.Value->ResetCheck();
	}
	
	for (auto etcItemEntry : EtcItemSlots)
	{
		if (etcItemEntry.Value)
			etcItemEntry.Value->ResetCheck();
	}
}

void UBJS_InventoryWidget::SetGold(int32 Gold)
{
	FString str = FString::FromInt(Gold);
	tb_gold->SetText(FText::FromString(str));
}

void UBJS_InventoryWidget::ViewInventoryEtc()
{
	InventoryModeState = InventoryModeState & InventoryMode::EMPTY;
	InventoryModeState += InventoryMode::ETC;
	SlotResetCheck();
	SetSlot();
	ugp_EquipItemSlots->SetVisibility(ESlateVisibility::Hidden);
	ugp_ItemSlots->SetVisibility(ESlateVisibility::Visible);
}

void UBJS_InventoryWidget::ViewInventoryEquip()
{
	InventoryModeState = InventoryModeState & InventoryMode::EMPTY;
	InventoryModeState += InventoryMode::EQUIP;
	SlotResetCheck();
	SetSlot();
	ugp_EquipItemSlots->SetVisibility(ESlateVisibility::Visible);
	ugp_ItemSlots->SetVisibility(ESlateVisibility::Hidden);
}

void UBJS_InventoryWidget::InvetoryReset()
{
	SlotResetCheck();
}

void UBJS_InventoryWidget::SellItem()
{
	auto instance = Cast<UBJS_GameInstance>(GetGameInstance());
	if (!instance) return;
	
	auto mode = Cast<ABJS_InGameMode>(GetWorld()->GetAuthGameMode());
	if (!mode) return;
	
	for (UWidget* Child : ugp_EquipItemSlots->GetAllChildren())
	{
		if (UBJS_ItemSlotWidget* ItemSlot = Cast<UBJS_ItemSlotWidget>(Child))
		{
			if (ItemSlot->GetCheck())
			{
				mode->SellEquipItems.Add(ItemSlot->GetEquip().UniqueId, ItemSlot->GetEquip());
				ItemSlot->ResetCheck();
			}
		}
		else
		{
			break;
		}
	}
	
	for (UWidget* Child : ugp_ItemSlots->GetAllChildren())
	{
		if (UBJS_ItemSlotWidget* ItemSlot = Cast<UBJS_ItemSlotWidget>(Child))
		{
			if (ItemSlot->GetCheck())
			{
				mode->SellEtcItems.Add(ItemSlot->GetEtc().ItemCode, ItemSlot->GetEtc());
				ItemSlot->ResetCheck();
			}
		}
		else
		{
			break;
		}
	}
	
	auto widget = CreateWidget<UUserWidget>(GetWorld(), instance->GetPrompt2());
	if (widget)
	{
		FString str = FString(TEXT("판매하시겠습니까?"));
		auto promptWidget = Cast<UBJS_PromptWidget2>(widget);
		promptWidget->AddToViewport();
		promptWidget->SetText(str);
		promptWidget->OnPromptOk.BindUObject(mode, &ABJS_InGameMode::SellItems);
		promptWidget->OnPromptCancle.BindUObject(mode, &ABJS_InGameMode::ResetSellItems);
	}
}
