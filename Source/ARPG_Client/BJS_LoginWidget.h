// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BJS_UserWidgetBase.h"
#include "BJS_LoginWidget.generated.h"

// 로그인성공 임시 체크용
DECLARE_DELEGATE(FOnLoginDelegate)

/**
 * 
 */
UCLASS()
class ARPG_CLIENT_API UBJS_LoginWidget : public UBJS_UserWidgetBase
{
	GENERATED_BODY()

public:
	UBJS_LoginWidget();

	virtual void NativeConstruct() override;
	
	FOnLoginDelegate OnLoginSuccess;

	virtual void BJS_UpdateWidget() override;
	
private:
	UFUNCTION()
	void OnLoginEvent();
	
	UFUNCTION()
	void LoginCheck(int32 result);
	
private:
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UEditableTextBox* tb_id;
	
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UEditableTextBox* tb_pw;
	
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UButton* btn_login;
	
	FString ID;
	FString PW;
};
