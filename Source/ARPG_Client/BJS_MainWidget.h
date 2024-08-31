// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BJS_UserWidgetBase.h"
#include "BJS_MainWidget.generated.h"

// 게임시작 임시 체크용
DECLARE_DELEGATE(FOnStartDelegate)

/**
 * 
 */
UCLASS()
class ARPG_CLIENT_API UBJS_MainWidget : public UBJS_UserWidgetBase
{
	GENERATED_BODY()

public:
	UBJS_MainWidget();
	FOnStartDelegate OnSelect;
	FOnStartDelegate OnCStore;
	FOnStartDelegate OnWStore;
	FOnStartDelegate OnStart;

	UFUNCTION()
	void OnSelectEvent();
	UFUNCTION()
	void OnCStoreEvent();
	UFUNCTION()
	void OnWStoreEvent();
	UFUNCTION()
	void OnWStartEvent();

	void SetLevel(int32 lv);
	void SetName(FString name);

	virtual void BJS_UpdateWidget() override;

protected:
	virtual void NativeConstruct() override;
	
  	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* btn_Select;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UButton* btn_C_Store;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UButton* btn_W_Store;
	
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UButton* btn_start;
	
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UTextBlock* tb_lv;
	
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UTextBlock* tb_name;
};
