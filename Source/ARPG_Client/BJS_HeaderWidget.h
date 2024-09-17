// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BJS_SubWidget.h"
#include "BJS_HeaderWidget.generated.h"

DECLARE_DELEGATE(FOnBack)

UCLASS()
class ARPG_CLIENT_API UBJS_HeaderWidget : public UBJS_SubWidget
{
	GENERATED_BODY()

public:
	UBJS_HeaderWidget();
	~UBJS_HeaderWidget();

	UFUNCTION()
	void OnBackEvent();
	
	FOnBack OnBack;

	virtual void BJS_InitWidget() override;
	virtual void BJS_SubUpdateWidget() override;
	void SetTextTitle(FString Title);
	void SetTextGold(int32 Gold);
	void SetTextCash(int32 Cash);

protected:
	virtual void NativeConstruct() override;
	
private:
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UButton* btn_back;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UTextBlock* tb_title;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UCanvasPanel* cp_gold;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UCanvasPanel* cp_cash;
	
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UTextBlock* tb_gold;
	
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UTextBlock* tb_cash;
};
