// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BJS_UserWidgetBase.h"
#include "BJS_InventoryWidget.generated.h"

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

	void UpdateEquipSlot(int32 InvenPos);
	void RemoveEquipSlot(int32 InvenPos);
	void UpdateEquippedSlot(int32 EquipPos);
	void RemoveEquippedSlot(int32 EquipPos);
	void UpdateEtcSlot(int32 InvenPos);
	void SetSlot();
	void SlotResetCheck();

	void SetBtnEquipImage(TObjectPtr<class UTexture2D> Image);
	void SetBtnEtcImage(TObjectPtr<class UTexture2D> Image);

	UFUNCTION()
	void ViewInventoryEtc();
	UFUNCTION()
	void ViewInventoryEquip();
	UFUNCTION()
	void InvetoryReset();
	UFUNCTION()
	void SellItem();
	UFUNCTION()
	void OnHiddenInventory();
	UFUNCTION()
	void OnItemEquip(int32 InvenPos, int32 EquipPos);

protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UBJS_HeaderWidget* header;
	
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
	class UBJS_StateWidget* wg_state;
	
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UBJS_ItemSlotWidget* slot_attsocket;
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UBJS_ItemSlotWidget* slot_spesocket;
	
	TArray<TObjectPtr<class UTexture2D>> EnableInventoryButtonImageList;
	TSubclassOf<class UBJS_ItemSlotWidget> ItemSlotClass;
	TArray<TObjectPtr<class UBJS_ItemSlotWidget>> EquipSlot;
	TArray<TObjectPtr<class UBJS_ItemSlotWidget>> EtcSlot;
	TMap<int32, TObjectPtr<class UBJS_ItemSlotWidget>> EquipItemSlots;
	TMap<int32, TObjectPtr<class UBJS_ItemSlotWidget>> EtcItemSlots;
	
	// 인벤토리 사이즈 고정
	const int32 InvenSize = 20;
	const int32 ColSize = 5;
	const int32 RowSize = 4;
	int32 InventoryModeState = 0;
};
