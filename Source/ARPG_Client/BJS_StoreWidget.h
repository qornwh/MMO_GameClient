// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BJS_UserWidgetBase.h"
#include "BJS_StoreWidget.generated.h"

class UBJS_SelectButton_Widget;
DECLARE_DELEGATE(FOnBack)

UCLASS()
class ARPG_CLIENT_API UBJS_StoreWidget : public UBJS_UserWidgetBase
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	
public:
	UBJS_StoreWidget();
	FOnBack OnBack;
	
	UFUNCTION()
	void OnBackEvent();

	virtual void BJS_UpdateWidget() override;
	virtual void BJS_PromptWidget(int32 code) override;
	void SetMode(int mode);
	void SetStoreImage(TArray<UTexture2D*>& Array);

private:
	void CurrentCharaterWeapon(bool IsCheck, UBJS_SelectButton_Widget* SBtn);

private:
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UBJS_HeaderWidget* header;
	
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UBJS_SelectButton_Widget* sbtn_1;
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UBJS_SelectButton_Widget* sbtn_2;
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UBJS_SelectButton_Widget* sbtn_3;

	int32 Mode = 0; // 0 : 캐릭터, 1 : 무기
};
