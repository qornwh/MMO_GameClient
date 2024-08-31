// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BJS_UserWidgetBase.h"
#include "BJS_GameUI.generated.h"

DECLARE_DELEGATE_TwoParams(FOnChatMessage, FString, int32)

/**
 * 
 */
UCLASS()
class ARPG_CLIENT_API UBJS_GameUI : public UBJS_UserWidgetBase
{
	GENERATED_BODY()

public:
	UBJS_GameUI();

	virtual void BJS_InitWidget() override;
	
	void SetImg();
	void SetName(FString Name);
	void SetLv(FString Lv);
	void SetExp(float Exp);
	void SetHp(float Hp);

	FOnChatMessage OnChatMessage;

	virtual void BJS_UpdateWidget() override;

	UFUNCTION()
	void CoolTimeUpdate(int32 KeyBind, float CoolTime);

private:
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UImage* img_charater;
	
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UTextBlock* tb_Name;
	
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UTextBlock* tb_lv;
	
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UProgressBar* pb_exp;
	
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UProgressBar* pb_hp;
	
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UButton* btn_mail;
	
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UEditableTextBox* tb_chat;
	
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UButton* btn_chat;
	
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UScrollBox* mtb_chatlog;
	
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UBJS_SkillManagerWidget* skill_manager;
};
