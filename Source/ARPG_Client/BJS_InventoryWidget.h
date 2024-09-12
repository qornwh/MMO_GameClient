// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BJS_UserWidgetBase.h"
#include "BJS_InventoryWidget.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_CLIENT_API UBJS_InventoryWidget : public UBJS_UserWidgetBase
{
	GENERATED_BODY()

	enum InventoryMode : int32
	{
		EQUIP = 0,
		ETC = 1,
		NOTSELL = 0x000FFFF,
		SELL = 0x0010000,
		EMPTY = 0xFFF0000
	};

public:
	UBJS_InventoryWidget();

	virtual void BJS_InitWidget() override;
	virtual void BJS_UpdateWidget() override;

	void AddEquipSlot(int32 EquipUnipeId);
	void UpdateEquipSlot(int32 EquipUnipeId);
	void RemoveEquipSlot(int32 EquipUnipeId);
	void UpdateEtcSlot(int32 EtcItemCode);
	void SetSlot();
	void SlotResetCheck();
	void SetGold(int32 Gold);

	UFUNCTION()
	void ViewInventoryEtc();
	UFUNCTION()
	void ViewInventoryEquip();
	UFUNCTION()
	void InvetoryReset();
	UFUNCTION()
	void SellItem();

private:
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UImage* img_charater;
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UUniformGridPanel* ugp_ItemSlots;
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UUniformGridPanel* ugp_EquipItemSlots;
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UButton* btn_equip;
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UButton* btn_etc;
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UButton* btn_check;
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UButton* btn_sell;
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UTextBlock* tb_gold;
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UBJS_StateWidget* wg_state;
	
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UBJS_ItemSlotWidget* slot_attsocket;
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UBJS_ItemSlotWidget* slot_spesocket;
	
	TSubclassOf<class UBJS_ItemSlotWidget> ItemSlotClass;
	TArray<class UBJS_ItemSlotWidget*> EquipSlot;
	TArray<class UBJS_ItemSlotWidget*> EtcSlot;
	TMap<int32, class UBJS_ItemSlotWidget*> EquipItemSlots;
	TMap<int32, class UBJS_ItemSlotWidget*> EtcItemSlots;
	
	int32 ColSize = 5;
	int32 RowSize = 4;
	int32 InventoryModeState = 0;

	// 인벤토리 사이즈 고정
	const int32 InvenSize = 20;
};
