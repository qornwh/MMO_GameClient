// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BJS_SubWidget.h"
#include "InventoryItem.h"
#include "Components/CheckBox.h"
#include "BJS_ItemSlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_CLIENT_API UBJS_ItemSlotWidget : public UBJS_SubWidget
{
	GENERATED_BODY()

public:
	UBJS_ItemSlotWidget();

	virtual void NativeConstruct() override;

	void SetEquip(EquipItem& Item);
	void SetEtc(EtcItem& Item);
	EquipItem& GetEquip();
	EtcItem& GetEtc();

	void SetImg(UTexture2D* Image);
	void SetCnt(int32 Cnt);
	void SetSlots(bool Flag);
	void ResetCheck();
	bool GetCheck() const { return cb_check->GetCheckedState() == ECheckBoxState::Checked; }
	void SetEmptyEquip();
	void SetEtcEquip();

	void SetSocket(bool Flag);
	UFUNCTION()
	void SendItemEquipped();

private:
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UCustomButton* btn_item;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UImage* img_item;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UTextBlock* tb_cnt;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UCheckBox* cb_check;

	// 일단 임시로 빈 아이템 슬롯 둔다
	EquipItem CurEquipItem{-1, -1, -1, 0, 0, 0, -1, 0};
	EtcItem CurEtcItem{-1, 0, 0,  -1};
};
