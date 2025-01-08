// Fill out your copyright notice in the Description page of Project Settings.


#include "BJS_SubInventoryWidget.h"

#include "BJS_GameInstance.h"
#include "BJS_InGameMode.h"
#include "BJS_ItemSlotWidget.h"
#include "Components/Button.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"

UBJS_SubInventoryWidget::UBJS_SubInventoryWidget()
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

void UBJS_SubInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UBJS_SubInventoryWidget::BJS_InitWidget()
{
	Super::BJS_InitWidget();

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

			// 일단 장비아이템만 
			// equipSlot->SendItem.BindUObject(this, &UBJS_SubInventoryWidget::OnSendItem);
			equipSlot->SetSocketType(ITEMSOCKETTYPE::SUBINVENTORY);
		}
	}
	SetSlot();
	ViewInventoryEquip();
	btn_equip->OnClicked.AddDynamic(this, &UBJS_SubInventoryWidget::ViewInventoryEquip);
	btn_etc->OnClicked.AddDynamic(this, &UBJS_SubInventoryWidget::ViewInventoryEtc);
}

void UBJS_SubInventoryWidget::BJS_SubUpdateWidget()
{
	Super::BJS_SubUpdateWidget();
}

void UBJS_SubInventoryWidget::SetBtnEquipImage(TObjectPtr<UTexture2D> Image)
{
	if (Image)
	{
		FSlateBrush Brush;
		Brush.SetResourceObject(Image);
		Brush.ImageSize = FVector2D(Image->GetSizeX(), Image->GetSizeY());

		FButtonStyle Style = btn_equip->GetStyle();
		Style.Normal.SetResourceObject(Image);

		btn_equip->SetStyle(Style);
	}
}

void UBJS_SubInventoryWidget::SetBtnEtcImage(TObjectPtr<UTexture2D> Image)
{
	if (Image)
	{
		FSlateBrush Brush;
		Brush.SetResourceObject(Image);
		Brush.ImageSize = FVector2D(Image->GetSizeX(), Image->GetSizeY());

		FButtonStyle Style = btn_etc->GetStyle();
		Style.Normal.SetResourceObject(Image);

		btn_etc->SetStyle(Style);
	}
}

void UBJS_SubInventoryWidget::SetSlot()
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

void UBJS_SubInventoryWidget::ReLoadSlot()
{
	ReSetSlot();
	auto instance = Cast<UBJS_GameInstance>(GetGameInstance());
	if (instance)
	{
		auto& EquipItems = instance->GetMyInventory()->GetInventoryEquipItemList();
		auto& EquipImageMap = instance->GetItemEquipIconImgMap();
		auto& EtcItems = instance->GetMyInventory()->GetInventoryEtcItemList();
		auto& EtcImageMap = instance->GetItemEtcIconImgMap();

		for (auto& Item : EquipItems)
		{
			int32 index = Item.InvenPos;
			if (ugp_EquipItemSlots->GetChildrenCount() > index)
			{
				UBJS_ItemSlotWidget* slot = Cast<UBJS_ItemSlotWidget>(ugp_EquipItemSlots->GetChildAt(index));
				if (slot && !slot->GetEquip().EqualEquipItem(Item))
				{
					slot->SetEquip(Item);
					slot->SetSocket(true);
					slot->SetImg(EquipImageMap[Item.ItemCode]);
				}
			}
		}

		for (auto& Item : EtcItems)
		{
			int32 index = Item.InvenPos;
			if (ugp_ItemSlots->GetChildrenCount() > index)
			{
				UBJS_ItemSlotWidget* slot = Cast<UBJS_ItemSlotWidget>(ugp_ItemSlots->GetChildAt(index));
				if (slot && !slot->GetEtc().EqualEtcItem(Item))
				{
					slot->SetEtc(Item);
					slot->SetSocket(true);
					slot->SetImg(EtcImageMap[Item.ItemCode]);
				}
			}
		}
	}
}

void UBJS_SubInventoryWidget::ReSetSlot()
{
	for (auto Child : ugp_EquipItemSlots->GetAllChildren())
	{
		UBJS_ItemSlotWidget* slot = Cast<UBJS_ItemSlotWidget>(Child);
		slot->SetSocket(false);
	}
	for (auto Child : ugp_ItemSlots->GetAllChildren())
	{
		UBJS_ItemSlotWidget* slot = Cast<UBJS_ItemSlotWidget>(Child);
		slot->SetSocket(false);
	}
}

void UBJS_SubInventoryWidget::ViewInventoryEtc()
{
	InventoryModeState = InventoryMode::ETC;
	ugp_EquipItemSlots->SetVisibility(ESlateVisibility::Hidden);
	ugp_ItemSlots->SetVisibility(ESlateVisibility::Visible);

	SetBtnEtcImage(EnableInventoryButtonImageList[1]);
	SetBtnEquipImage(EnableInventoryButtonImageList[0]);
}

void UBJS_SubInventoryWidget::ViewInventoryEquip()
{
	InventoryModeState = InventoryMode::EQUIP;
	ugp_EquipItemSlots->SetVisibility(ESlateVisibility::Visible);
	ugp_ItemSlots->SetVisibility(ESlateVisibility::Hidden);

	SetBtnEquipImage(EnableInventoryButtonImageList[1]);
	SetBtnEtcImage(EnableInventoryButtonImageList[0]);
}

// void UBJS_SubInventoryWidget::OnSendItem(int32 EquipId)
// {
// 	auto mode = Cast<ABJS_InGameMode>(GetWorld()->GetAuthGameMode());
// 	if (mode)
// 	{
// 		if (EquipId > 0)
// 		{
// 			auto& EquipItems = mode->GetMyInventory()->GetEquipItems();
// 			if (EquipItems.Contains(EquipId))
// 			{
// 				auto Item = EquipItems.Find(EquipId);
// 				UE_LOG(LogTemp, Log, TEXT("메일 아이템 추가 !!! %d %d"), EquipId, Item->ItemCode);
// 			}
// 		}
// 	}
// }
