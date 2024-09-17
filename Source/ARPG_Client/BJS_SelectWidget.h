// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BJS_UserWidgetBase.h"
#include "BJS_SelectWidget.generated.h"

class UBJS_SelectButton_Widget;
DECLARE_DELEGATE(FOnSelect)
DECLARE_DELEGATE(FOnBack)

UCLASS()
class ARPG_CLIENT_API UBJS_SelectWidget : public UBJS_UserWidgetBase
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	
public:
	UBJS_SelectWidget();
	FOnSelect OnSelect;
	FOnBack OnBack;

	UFUNCTION()
	void OnSelectEvent();
	UFUNCTION()
	void OnBackEvent();
	UFUNCTION()
	void UpdateCurrentType();

	virtual void BJS_UpdateWidget() override;
	void SetSelectImage(TArray<UTexture2D*>& CharaterArray, TArray<UTexture2D*>& WeaponArray);

private:
	void CurrentCharaterWeapon(bool IsCheck, int32 Idx, int32 CurIdx, class UBJS_SelectButton_Widget* SBtn);

private:
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UBJS_HeaderWidget* header;
	
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UBJS_SelectButton_Widget* sbtn_c1;
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UBJS_SelectButton_Widget* sbtn_c2;
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UBJS_SelectButton_Widget* sbtn_c3;
	
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UBJS_SelectButton_Widget* sbtn_w1;
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UBJS_SelectButton_Widget* sbtn_w2;
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UBJS_SelectButton_Widget* sbtn_w3;

	// 없을 경우 비활성화
	float opacity = 0.5;
};
