// Fill out your copyright notice in the Description page of Project Settings.


#include "BJS_InventoryWidget.h"

#include "BJS_CharaterState.h"
#include "BJS_GameInstance.h"
#include "BJS_HeaderWidget.h"
#include "BJS_InGameMode.h"
#include "BJS_ItemSlotWidget.h"
#include "BJS_PromptWidget2.h"
#include "BJS_StateWidget.h"
#include "InventoryItem.h"
#include "Components/Button.h"
#include "Components/GridPanel.h"
#include "Components/Image.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"

UBJS_InventoryWidget::UBJS_InventoryWidget()
{
	static ConstructorHelpers::FClassFinder<UUserWidget> ITEM_SLOT_HUD(
		TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/MyGame/UMG/BJS_SWBP_ItemSlot.BJS_SWBP_ItemSlot_C'"));
	check(ITEM_SLOT_HUD.Class);

	static ConstructorHelpers::FObjectFinder<UTexture2D> BTN_INVENTORY_OFF_IMAGE(
		TEXT("/Script/Engine.Texture2D'/Game/Cyberpunk_RPG_GUI_Resources/SlicedElements/05Hero_Equipment/Btn_SubMenu_n.Btn_SubMenu_n'"));
	check(BTN_INVENTORY_OFF_IMAGE.Object);

	static ConstructorHelpers::FObjectFinder<UTexture2D> BTN_INVENTORY_ON_IMAGE(
		TEXT("/Script/Engine.Texture2D'/Game/Cyberpunk_RPG_GUI_Resources/SlicedElements/05Hero_Equipment/Btn_SubMenu_s.Btn_SubMenu_s'"));
	check(BTN_INVENTORY_ON_IMAGE.Object);

	ItemSlotClass = ITEM_SLOT_HUD.Class;
	InventoryModeState = InventoryMode::EQUIP;
	EnableInventoryButtonImageList.Add(BTN_INVENTORY_OFF_IMAGE.Object);
	EnableInventoryButtonImageList.Add(BTN_INVENTORY_ON_IMAGE.Object);
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

			equipSlot->SendItem.BindUObject(this, &UBJS_InventoryWidget::OnItemEquip);
		}
	}

	btn_equip->OnClicked.AddDynamic(this, &UBJS_InventoryWidget::ViewInventoryEquip);
	btn_etc->OnClicked.AddDynamic(this, &UBJS_InventoryWidget::ViewInventoryEtc);
	btn_sell->OnClicked.AddDynamic(this, &UBJS_InventoryWidget::SellItem);
	btn_check->OnClicked.AddDynamic(this, &UBJS_InventoryWidget::InvetoryReset);
	slot_attsocket->SendItem.BindUObject(this, &UBJS_InventoryWidget::OnItemEquip);
	slot_spesocket->SendItem.BindUObject(this, &UBJS_InventoryWidget::OnItemEquip);
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

		header->SetTextCash(instance->GetCash());
		header->SetTextGold(instance->GetMyInventory()->GetGold());
	}
}

void UBJS_InventoryWidget::UpdateEquipSlot(int32 InvenPos)
{
	auto instance = Cast<UBJS_GameInstance>(GetGameInstance());
	if (instance)
	{
		auto& item = instance->GetMyInventory()->GetInventoryEquipItemList()[InvenPos];
		if (!item.IsEmpty())
		{
			auto& ImageMap = instance->GetItemEquipIconImgMap();
			auto slot = EquipSlot[InvenPos];
			slot->SetEquip(item);
			slot->SetSlots(true);
			slot->SetImg(ImageMap[item.ItemCode]);
		}
		else
		{
			RemoveEquipSlot(InvenPos);
		}
	}
}

void UBJS_InventoryWidget::RemoveEquipSlot(int32 InvenPos)
{
	auto instance = Cast<UBJS_GameInstance>(GetGameInstance());
	if (instance)
	{
		auto slot = EquipSlot[InvenPos];
		slot->SetSlots(false);
		slot->SetImg(nullptr);
	}
}

void UBJS_InventoryWidget::UpdateEquippedSlot(int32 EquipPos)
{
	auto instance = Cast<UBJS_GameInstance>(GetGameInstance());
	if (instance)
	{
		auto& item = instance->GetMyInventory()->GetEquippedItemList()[EquipPos];
		if (!item.IsEmpty())
		{
			auto& ImageMap = instance->GetItemEquipIconImgMap();
			UBJS_ItemSlotWidget* slot;
			if (EquipPos == 1)
			{
				slot = slot_attsocket;
			}
			else
			{
				slot = slot_spesocket;
			}
			slot->SetEquip(item);
			slot->SetSlots(true);
			slot->SetImg(ImageMap[item.ItemCode]);
		}
		else
		{
			RemoveEquippedSlot(EquipPos);
		}
	}
}

void UBJS_InventoryWidget::RemoveEquippedSlot(int32 EquipPos)
{
	auto instance = Cast<UBJS_GameInstance>(GetGameInstance());
	if (instance)
	{
		UBJS_ItemSlotWidget* slot;
		if (EquipPos == 1)
		{
			slot = slot_attsocket;
		}
		else
		{
			slot = slot_spesocket;
		}
		slot->SetSlots(false);
		slot->SetImg(nullptr);
	}
}

void UBJS_InventoryWidget::UpdateEtcSlot(int32 InvenPos)
{
	auto instance = Cast<UBJS_GameInstance>(GetGameInstance());
	if (instance)
	{
		auto& item = instance->GetMyInventory()->GetInventoryEtcItemList()[InvenPos];
		if (!item.IsEmpty())
		{
			auto& ImageMap = instance->GetItemEtcIconImgMap();
			auto slot = EtcSlot[InvenPos];
			slot->SetEtc(item);
			slot->SetSlots(true);
			slot->SetImg(ImageMap[item.ItemCode]);
		}
		else
		{
			auto slot = EtcSlot[InvenPos];
			slot->SetSlots(false);
			slot->SetImg(nullptr);
		}
	}
}

void UBJS_InventoryWidget::SetSlot()
{
	int32 row = 0;
	int32 col = 0;
	ugp_EquipItemSlots->ClearChildren();
	ugp_ItemSlots->ClearChildren();

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

void UBJS_InventoryWidget::SetBtnEquipImage(TObjectPtr<UTexture2D> Image)
{
	if (Image)
	{
		FSlateBrush Brush;
		Brush.SetResourceObject(Image);
		Brush.ImageSize = FVector2D(Image->GetSizeX(), Image->GetSizeY());

		FButtonStyle Style = btn_equip->GetStyle();

		Style.Normal.SetResourceObject(Image);
		// Style.Normal = Brush;

		btn_equip->SetStyle(Style);
	}
}

void UBJS_InventoryWidget::SetBtnEtcImage(TObjectPtr<UTexture2D> Image)
{
	if (Image)
	{
		FSlateBrush Brush;
		Brush.SetResourceObject(Image);
		Brush.ImageSize = FVector2D(Image->GetSizeX(), Image->GetSizeY());

		FButtonStyle Style = btn_etc->GetStyle();

		Style.Normal.SetResourceObject(Image);
		// Style.Normal = Brush;

		btn_etc->SetStyle(Style);
	}
}

void UBJS_InventoryWidget::ViewInventoryEtc()
{
	SlotResetCheck();
	InventoryModeState = InventoryMode::ETC;
	ugp_EquipItemSlots->SetVisibility(ESlateVisibility::Hidden);
	ugp_ItemSlots->SetVisibility(ESlateVisibility::Visible);

	SetBtnEtcImage(EnableInventoryButtonImageList[1]);
	SetBtnEquipImage(EnableInventoryButtonImageList[0]);
}

void UBJS_InventoryWidget::ViewInventoryEquip()
{
	SlotResetCheck();
	InventoryModeState = InventoryMode::EQUIP;
	ugp_EquipItemSlots->SetVisibility(ESlateVisibility::Visible);
	ugp_ItemSlots->SetVisibility(ESlateVisibility::Hidden);

	SetBtnEquipImage(EnableInventoryButtonImageList[1]);
	SetBtnEtcImage(EnableInventoryButtonImageList[0]);
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
				mode->SellEquipItems.Add(ItemSlot->GetEquip().InvenPos, ItemSlot->GetEquip());
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

	auto widget = mode->OpenPromptWidget2();
	FString str = FString(TEXT("판매하시겠습니까?"));
	widget->SetText(str);
	widget->OnPromptOk.BindUObject(mode, &ABJS_InGameMode::SellItems);
	widget->OnPromptCancle.BindUObject(mode, &ABJS_InGameMode::ResetSellItems);
}

void UBJS_InventoryWidget::OnHiddenInventory()
{
	auto mode = Cast<ABJS_InGameMode>(GetWorld()->GetAuthGameMode());
	if (mode)
	{
		mode->ChangeInventoryUI();
	}
}

void UBJS_InventoryWidget::OnItemEquip(int32 InvenPos, int32 EquipPos)
{
	auto mode = Cast<ABJS_InGameMode>(GetWorld()->GetAuthGameMode());
	if (mode)
	{
		mode->SendEquippedItem(InvenPos, EquipPos);
	}
}

void UBJS_InventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	header->OnBack.BindUObject(this, &UBJS_InventoryWidget::OnHiddenInventory);
	header->SetTextTitle("Inventory");
}
