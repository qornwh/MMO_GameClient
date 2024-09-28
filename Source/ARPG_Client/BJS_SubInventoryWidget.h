// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BJS_SubWidget.h"
#include "BJS_SubInventoryWidget.generated.h"


UCLASS()
class ARPG_CLIENT_API UBJS_SubInventoryWidget : public UBJS_SubWidget
{
	GENERATED_BODY()

	enum InventoryMode : int32
	{
		EQUIP = 0,
		ETC = 1,
	};

public:
	UBJS_SubInventoryWidget();
	virtual void NativeConstruct() override;
	virtual void BJS_InitWidget() override;
	virtual void BJS_SubUpdateWidget() override;
	void SetBtnEquipImage(TObjectPtr<class UTexture2D> Image);
	void SetBtnEtcImage(TObjectPtr<class UTexture2D> Image);
	void SetSlot();
	void ReLoadSlot();
	void ReSetSlot();
	
	UFUNCTION()
	void ViewInventoryEtc();
	UFUNCTION()
	void ViewInventoryEquip();
	UFUNCTION()
	void OnSendItem(int32 EquipId);

private:
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UUniformGridPanel* ugp_ItemSlots;
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UUniformGridPanel* ugp_EquipItemSlots;
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UButton* btn_equip;
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UButton* btn_etc;
	
	TSubclassOf<class UBJS_ItemSlotWidget> ItemSlotClass;
	TArray<TObjectPtr<class UTexture2D>> EnableInventoryButtonImageList;
	
	TArray<TObjectPtr<class UBJS_ItemSlotWidget>> EquipSlot;
	TArray<TObjectPtr<class UBJS_ItemSlotWidget>> EtcSlot;
	const int32 ColSize = 5;
	const int32 RowSize = 4;
	int32 InventoryModeState = 0;
};
