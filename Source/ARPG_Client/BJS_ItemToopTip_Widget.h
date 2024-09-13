// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BJS_SubWidget.h"
#include "InventoryItem.h"
#include "BJS_ItemToopTip_Widget.generated.h"

UCLASS()
class ARPG_CLIENT_API UBJS_ItemToopTip_Widget : public UBJS_SubWidget
{
	GENERATED_BODY()

public:
	UBJS_ItemToopTip_Widget();
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnClose();

	void SetEquipItem(EquipItem& NewEquipItem);
	void SetEtcItem(EtcItem& NewEtcItem);
	void ResetInfo();

	void SetImg(UTexture2D* Image);
	void SetName(FString Name);
	void SetType(int32 Type);
	void SetAttack(int32 Attack);
	void SetSpeed(int32 Speed);
	void SetDescription(FString Description);

private:
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UButton* btn_close;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UImage* img_icon;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UTextBlock* tb_name;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UTextBlock* tb_type;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UTextBlock* tb_attack;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UTextBlock* tb_speed;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UTextBlock* n_tb_attack;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UTextBlock* n_tb_speed;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UMultiLineEditableTextBox* mtb_description;

	// 일단 임시로 빈 아이템 슬롯 둔다
	EquipItem CurEquipItem{-1, -1, -1, 0, 0, 0, -1, 0};
	EtcItem CurEtcItem{-1, 0, 0, -1};

	ToolTipType type = ToolTipType::NOT_DATA;
};
