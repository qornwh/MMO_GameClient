// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BJS_SubWidget.h"
#include "MailBox.h"
#include "BJS_MailSlot.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_CLIENT_API UBJS_MailSlot : public UBJS_SubWidget
{
	GENERATED_BODY()

public:
	UBJS_MailSlot();
	~UBJS_MailSlot();
	void SetSlot(bool Flag);
	void SetParentWidget(class UBJS_MailWidget* Widget);
	void SetMail(Mail& NewMail);

	UFUNCTION()
	void SelectMail();

protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UTextBlock* tb_title;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UCheckBox* cb_check;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UButton* btn_select;

	class UBJS_MailWidget* parentWidget = nullptr;
	Mail CurrentMail{-1, -1, 0, -1, -1, -1, -1, "", ""};
};
