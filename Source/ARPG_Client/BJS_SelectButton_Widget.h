// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BJS_UserWidgetBase.h"
#include "BJS_SelectButton_Widget.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_CLIENT_API UBJS_SelectButton_Widget : public UBJS_UserWidgetBase
{
	GENERATED_BODY()

public:
	UBJS_SelectButton_Widget();
	virtual void BJS_UpdateWidget() override;

	void SetType(int32 CWType);
	void SetSelectBtn(bool Select, bool Own);
	void SetBuyBtn(bool Own);
	void SetImageBtn(UTexture2D* Image);
	bool GetSelect();
	bool GetOwn();
	class UButton* GetBtn();
	void SetOpacity();
	
	UFUNCTION()
	void CreateCharater(FString Name);
	UFUNCTION()
	void OnSelectBtn();
	UFUNCTION()
	void OnBuyBtn();
	
private:
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UButton* btn;
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UTextBlock* tb;

	bool IsSelect = false;
	bool IsOwn = false;
	int32 Type = 0;
	float Opactiy = 0.5f;
	FString Select_Select_Str = TEXT("선택됨");
	FString Select_Own_Str = TEXT("보유중");
	FString Select_No_Str = TEXT("보유중이지 않음");
	FString Create_Name_Str = TEXT("캐릭터 이름 생성");
	int32 pay = 2500; // 일단 고정

	UPROPERTY(EditAnywhere, Category = "Menus")
	TSubclassOf<class UBJS_CreateChareter_Prompt_Widget> CreateCharecterPromptClass;
};
