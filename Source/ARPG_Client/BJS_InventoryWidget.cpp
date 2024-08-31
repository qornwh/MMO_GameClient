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
			auto slot = CreateWidget<UBJS_ItemSlotWidget>(GetWorld(), ItemSlotClass);

			UUniformGridSlot* GridSlot = ugp_ItemSlots->AddChildToUniformGrid(slot, i, j);
			if (GridSlot)
			{
				GridSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
				GridSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);
			}
		}
	}

	btn_equip->OnClicked.AddDynamic(this, &UBJS_InventoryWidget::ViewInventoryEquip);
	btn_etc->OnClicked.AddDynamic(this, &UBJS_InventoryWidget::ViewInventoryEtc);
	btn_sell->OnClicked.AddDynamic(this, &UBJS_InventoryWidget::SellItem);
	btn_check->OnClicked.AddDynamic(this, &UBJS_InventoryWidget::InvetoryReset);
}

void UBJS_InventoryWidget::BJS_UpdateWidget()
{
	Super::BJS_UpdateWidget();

	if ((InventoryModeState & NOTSELL) == InventoryMode::EQUIP)
	{
		UpdateEquipInventory();
	}
	else
	{
		UpdateEtcInventory();
	}

	auto instance = Cast<UBJS_GameInstance>(GetGameInstance());
	if (instance)
	{
		wg_state->SetAttack(instance->GetMyState()->GetAttack());
		wg_state->SetSpeed(instance->GetMyState()->GetSpeed());
	}
}

void UBJS_InventoryWidget::UpdateEquipInventory()
{
	auto instance = Cast<UBJS_GameInstance>(GetGameInstance());
	if (instance)
	{
		auto& ImageMap = instance->GetItemEquipIconImgMap();
		auto& items = instance->GetMyInventory()->GetEquipItems();
		int32 idx = 0;
		for (auto& itemEntry : items)
		{
			for (auto& item : itemEntry.Value)
			{
				int32 Code = item.Code;
				auto Current = GetWidgetAtGridPosition(idx / ColSize, idx % ColSize);
				Current->SetImg(ImageMap[Code]);
				Current->SetCnt(-1);
				Current->SetEquip(Code);
				idx++;
			}
		}
		GridClear(idx);
		SetGold(instance->GetMyInventory()->GetGold());
	}
}

void UBJS_InventoryWidget::UpdateEtcInventory()
{
	auto instance = Cast<UBJS_GameInstance>(GetGameInstance());
	if (instance)
	{
		auto& ImageMap = instance->GetItemEtcIconImgMap();
		auto& items = instance->GetMyInventory()->GetEtcItems();
		int32 idx = 0;
		for (auto& item : items)
		{
			int32 Code = item.Value.Code;
			int32 Cnt = item.Value.Count;
			auto Current = GetWidgetAtGridPosition(idx / ColSize, idx % ColSize);
			Current->SetImg(ImageMap[Code]);
			Current->SetCnt(Cnt);
			Current->SetEquip(Code, Cnt);
			idx++;
		}
		GridClear(idx);
		SetGold(instance->GetMyInventory()->GetGold());
	}
}

void UBJS_InventoryWidget::SetGold(int32 Gold)
{
	FString str = FString::FromInt(Gold);
	tb_gold->SetText(FText::FromString(str));
}

UBJS_ItemSlotWidget* UBJS_InventoryWidget::GetWidgetAtGridPosition(int32 I, int32 J)
{
	if (!ugp_ItemSlots) return nullptr;

	for (UWidget* Child : ugp_ItemSlots->GetAllChildren())
	{
		if (UUniformGridSlot* GridSlot = Cast<UUniformGridSlot>(Child->Slot))
		{
			if (GridSlot->GetRow() == I && GridSlot->GetColumn() == J)
			{
				return Cast<UBJS_ItemSlotWidget>(Child);
			}
		}
	}
	return nullptr;
}

void UBJS_InventoryWidget::GridClear(int32 Idx)
{
	for (UWidget* Child : ugp_ItemSlots->GetAllChildren())
	{
		if (UUniformGridSlot* GridSlot = Cast<UUniformGridSlot>(Child->Slot))
		{
			int curIdx = GridSlot->GetRow() * ColSize + GridSlot->GetColumn();
			if (curIdx >= Idx)
			{
				Cast<UBJS_ItemSlotWidget>(Child)->ShowImg(false);
			}
		}
	}
}

void UBJS_InventoryWidget::SlotResetCheck()
{
	for (UWidget* Child : ugp_ItemSlots->GetAllChildren())
	{
		if (UBJS_ItemSlotWidget* ItemSlot = Cast<UBJS_ItemSlotWidget>(Child))
		{
			if (ItemSlot->GetCheck())
			{
				ItemSlot->ResetCheck();
			}
		}
		else
		{
			break;
		}
	}
}

void UBJS_InventoryWidget::ViewInventoryEtc()
{
	InventoryModeState = InventoryModeState & InventoryMode::EMPTY;
	InventoryModeState += InventoryMode::ETC;
	SlotResetCheck();
}

void UBJS_InventoryWidget::ViewInventoryEquip()
{
	InventoryModeState = InventoryModeState & InventoryMode::EMPTY;
	InventoryModeState += InventoryMode::EQUIP;
	SlotResetCheck();
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
	
	for (UWidget* Child : ugp_ItemSlots->GetAllChildren())
	{
		if (UBJS_ItemSlotWidget* ItemSlot = Cast<UBJS_ItemSlotWidget>(Child))
		{
			if (ItemSlot->GetCheck())
			{
				if ((InventoryModeState & NOTSELL) == InventoryMode::EQUIP)
				{
					if (!mode->SellEquipItems.Contains(ItemSlot->GetEquip().Code))
						mode->SellEquipItems.Add(ItemSlot->GetEquip().Code, TArray<EquipItem>());
					mode->SellEquipItems.Find(ItemSlot->GetEquip().Code)->Add(ItemSlot->GetEquip());
				}
				else
				{
					if (!mode->SellEquipItems.Contains(ItemSlot->GetEquip().Code))
						mode->SellEtcItems.Add(ItemSlot->GetEquip().Code, ItemSlot->GetEtc());
				}
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
		auto promptWidget = Cast<UBJS_PromptWidget2>(widget);
		promptWidget->AddToViewport();
		FString str = FString(TEXT("판매하시겠습니까?"));
		promptWidget->SetText(str);
		promptWidget->OnPromptOk.BindUObject(mode, &ABJS_InGameMode::SellItems);
		promptWidget->OnPromptCancle.BindUObject(mode, &ABJS_InGameMode::ResetSellItems);
	}
}
