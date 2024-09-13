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
			
			wg_state->SetName(instance->GetMyState()->GetName());
		}
	}

	for (int32 i = 0; i < RowSize; i++)
	{
		for (int32 j = 0; j < ColSize; j++)
		{
			auto equipSlot = CreateWidget<UBJS_ItemSlotWidget>(GetWorld(), ItemSlotClass);
			auto etcSlot = CreateWidget<UBJS_ItemSlotWidget>(GetWorld(), ItemSlotClass);
			equipSlot->SetSlots(false);
			etcSlot->SetSlots(false);
			EquipSlot.Add(equipSlot);
			EtcSlot.Add(etcSlot);
		}
	}

	btn_equip->OnClicked.AddDynamic(this, &UBJS_InventoryWidget::ViewInventoryEquip);
	btn_etc->OnClicked.AddDynamic(this, &UBJS_InventoryWidget::ViewInventoryEtc);
	btn_sell->OnClicked.AddDynamic(this, &UBJS_InventoryWidget::SellItem);
	btn_check->OnClicked.AddDynamic(this, &UBJS_InventoryWidget::InvetoryReset);
	slot_attsocket->SetSocket(false);
	slot_spesocket->SetSocket(false);
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
		wg_state->SetLv(instance->GetMyState()->GetLv());

		SetGold(instance->GetMyInventory()->GetGold());
	}
}

void UBJS_InventoryWidget::AddEquipSlot(int32 EquipUnipeId)
{
	auto instance = Cast<UBJS_GameInstance>(GetGameInstance());
	if (instance)
	{
		if (instance->GetMyInventory()->GetEquipItems().Contains(EquipUnipeId))
		{
			auto item = instance->GetMyInventory()->GetEquipItems().Find(EquipUnipeId);
			auto& ImageMap = instance->GetItemEquipIconImgMap();

			if (item->IsEquip == 0)
			{
				int32 idx = item->Position;
				if (InvenSize > idx)
				{
					auto slot = EquipSlot[idx];
					slot->SetEquip(*item);
					slot->SetSlots(true);
					slot->SetImg(ImageMap[item->ItemCode]);
					EquipItemSlots.Add(EquipUnipeId, slot);
				}
			}
			else if (item->IsEquip == 1)
			{
				if (item->EquipType == 1)
				{
					slot_attsocket->SetEquip(*item);
					slot_attsocket->SetSocket(true);
					slot_attsocket->SetImg(ImageMap[item->ItemCode]);
				}
				else if (item->EquipType == 2)
				{
					slot_spesocket->SetEquip(*item);
					slot_spesocket->SetSocket(true);
					slot_spesocket->SetImg(ImageMap[item->ItemCode]);
				}

				if (EquipItemSlots.Contains(EquipUnipeId))
				{
					auto slot = EquipItemSlots.Find(EquipUnipeId);
					EquipItemSlots.Remove(EquipUnipeId);
					(*slot)->SetSlots(false);
				}
			}
		}
	}
}

void UBJS_InventoryWidget::UpdateEquipSlot(int32 EquipUnipeId)
{
	auto instance = Cast<UBJS_GameInstance>(GetGameInstance());
	if (instance)
	{
		if (instance->GetMyInventory()->GetEquipItems().Contains(EquipUnipeId))
		{
			auto item = instance->GetMyInventory()->GetEquipItems().Find(EquipUnipeId);
			int32 isEquip = item->IsEquip;
			if (isEquip == 1)
			{
				// 장착
				if (EquipItemSlots.Contains(EquipUnipeId))
				{
					AddEquipSlot(EquipUnipeId);
				}
			}
			else
			{
				// 장착해제
				if (!EquipItemSlots.Contains(EquipUnipeId))
				{
					RemoveEquipSlot(EquipUnipeId);
					AddEquipSlot(EquipUnipeId);
				}
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
			(*slot)->SetSlots(false);
		}
		else
		{
			// 장착 해제
			auto item = instance->GetMyInventory()->GetEquipItems().Find(EquipUnipeId);
			if (item->EquipType == 1)
			{
				if (slot_attsocket->GetEquip().UniqueId == EquipUnipeId)
				{
					slot_attsocket->SetSocket(false);
					slot_attsocket->SetEmptyEquip();
				}
			}
			else if (item->EquipType == 2)
			{
				if (slot_spesocket->GetEquip().UniqueId == EquipUnipeId)
				{
					slot_spesocket->SetSocket(false);
					slot_spesocket->SetEmptyEquip();
				}
			}
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
				auto item = instance->GetMyInventory()->GetEtcItems().Find(EtcItemCode);
				int idx = item->Position;
				if (InvenSize > idx)
				{
					auto slot = EtcSlot[idx];
					auto& ImageMap = instance->GetItemEtcIconImgMap();
					slot->SetEtc(*item);
					slot->SetSlots(true);
					slot->SetImg(ImageMap[EtcItemCode]);
					slot->SetCnt(item->Count);
					EtcItemSlots.Add(EtcItemCode, slot);
				}
			}
		}
		else
		{
			// 삭제
			auto slot = EtcItemSlots.Find(EtcItemCode);
			EtcItemSlots.Remove(EtcItemCode);
			(*slot)->SetSlots(false);
		}
	}
}

void UBJS_InventoryWidget::SetSlot()
{
	int32 row = 0;
	int32 col = 0;
	ugp_EquipItemSlots->ClearChildren();

	for (auto empty : EquipSlot)
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

	ugp_ItemSlots->ClearChildren();

	row = 0;
	col = 0;
	for (auto empty : EtcSlot)
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

void UBJS_InventoryWidget::SlotResetCheck()
{
	if ((InventoryModeState && InventoryMode::EQUIP) == InventoryMode::EQUIP)
	{
		for (auto equipItemEntry : EquipItemSlots)
		{
			if (equipItemEntry.Value)
				equipItemEntry.Value->ResetCheck();
		}
	}
	else if ((InventoryModeState && InventoryMode::ETC) == InventoryMode::ETC)
	{
		for (auto etcItemEntry : EtcItemSlots)
		{
			if (etcItemEntry.Value)
				etcItemEntry.Value->ResetCheck();
		}
	}
}

void UBJS_InventoryWidget::SetGold(int32 Gold)
{
	FString str = FString::FromInt(Gold);
	tb_gold->SetText(FText::FromString(str));
}

void UBJS_InventoryWidget::ViewInventoryEtc()
{
	SlotResetCheck();
	InventoryModeState = InventoryMode::ETC;
	ugp_EquipItemSlots->SetVisibility(ESlateVisibility::Hidden);
	ugp_ItemSlots->SetVisibility(ESlateVisibility::Visible);
}

void UBJS_InventoryWidget::ViewInventoryEquip()
{
	SlotResetCheck();
	InventoryModeState = InventoryMode::EQUIP;
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
