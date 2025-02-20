// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CustomButton.h"
#include "InventoryItem.h"
#include "Components/CheckBox.h"
#include "BJS_ItemSlotWidget.generated.h"

DECLARE_DELEGATE_TwoParams(FSendItem, int32, int32); 
	
enum ITEMSOCKETTYPE : int32
{
	NONE = 0,
	ARMER = 1,
	SENDMAIL = 2,
	SUBINVENTORY = 3
};

UCLASS()
class ARPG_CLIENT_API UBJS_ItemSlotWidget : public UCustomButton
{
	GENERATED_BODY()

public:
	UBJS_ItemSlotWidget();

	void SetEquip(EquipItem& Item);
	void SetEtc(EtcItem& Item);
	EquipItem& GetEquip();
	EtcItem& GetEtc();
	void SetImg(UTexture2D* Image);
	void SetCnt(int32 Cnt);
	void SetSlots(bool Flag);
	void SetSocket(bool Flag);
	void SetRecive(bool Flag);
	void ResetCheck();
	bool GetCheck() const { return cb_check->GetCheckedState() == ECheckBoxState::Checked; }
	void SetEmptyEquip();
	void SetEtcEquip();
	void SetSocketType(ITEMSOCKETTYPE SocketType);
	void SetSocketPosition(int32 Position);
	
	UFUNCTION()
	void OnSendItem();
	UFUNCTION()
	void SendToolTipItemInfo();

	FSendItem SendItem;

protected:
	virtual void NativeConstruct() override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

private:
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UImage* img_item;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UTextBlock* tb_cnt;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UCheckBox* cb_check;

	// 일단 임시로 빈 아이템 슬롯 둔다
	EquipItem CurEquipItem{-1, -1, -1, -1, -1, -1};
	EtcItem CurEtcItem{-1, 0, 0,  -1};
	ToolTipType Type = ToolTipType::NOT_DATA;
	ITEMSOCKETTYPE CurSocketType = ITEMSOCKETTYPE::NONE;
	int32 CurSocketPosition = 0;
};
