// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BJS_SubWidget.h"
#include "InventoryItem.h"
#include "Components/CheckBox.h"
#include "BJS_ItemSlotWidget.generated.h"

DECLARE_DELEGATE_OneParam(FOnWidgetClicked, UBJS_ItemSlotWidget*);

/**
 * 
 */
UCLASS()
class ARPG_CLIENT_API UBJS_ItemSlotWidget : public UBJS_SubWidget
{
	GENERATED_BODY()
	
public:
	UBJS_ItemSlotWidget();
	
	virtual void BJS_InitWidget() override;
	virtual void BJS_SubUpdateWidget() override;

	void SetEquip(int32 Code);
	void SetEquip(int32 Code, int32 Count);
	EquipItem& GetEquip();
	EtcItem& GetEtc();
	
	void SetImg(UTexture2D* Image);
	void SetCnt(int32 Cnt);
	void ShowImg(bool Flag);
	class UButton* GetButton() { return btn_item; }
	void ResetCheck();
	bool GetCheck() { return cb_check->GetCheckedState() == ECheckBoxState::Checked; };
	
	// 델리게이트 인스턴스
	FOnWidgetClicked OnClicked;
	
	UFUNCTION()
	void OnMouseClickHandler(FGeometry MyGeometry, const FPointerEvent& MouseEvent);

protected:
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	
private:
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UButton* btn_item;
	
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UImage* img_item;
	
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UTextBlock* tb_cnt;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UCheckBox* cb_check;

	EquipItem EquipItem;
	EtcItem EtcItem;
};
