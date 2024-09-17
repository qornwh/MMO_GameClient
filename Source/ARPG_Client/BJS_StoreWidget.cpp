// Fill out your copyright notice in the Description page of Project Settings.


#include "BJS_StoreWidget.h"

#include "BJS_GameInstance.h"
#include "BJS_HeaderWidget.h"
#include "BJS_PromptWidget.h"
#include "BJS_SelectButton_Widget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UBJS_StoreWidget::NativeConstruct()
{
	Super::NativeConstruct();

	header->OnBack.BindUObject(this, &UBJS_StoreWidget::OnBackEvent);
	header->SetTextTitle("Store");

	sbtn_1->GetBtn()->OnClicked.AddDynamic(sbtn_1, &UBJS_SelectButton_Widget::OnBuyBtn);
	sbtn_2->GetBtn()->OnClicked.AddDynamic(sbtn_2, &UBJS_SelectButton_Widget::OnBuyBtn);
	sbtn_3->GetBtn()->OnClicked.AddDynamic(sbtn_3, &UBJS_SelectButton_Widget::OnBuyBtn);
}

UBJS_StoreWidget::UBJS_StoreWidget()
{
}

void UBJS_StoreWidget::OnBackEvent()
{
	OnBack.Execute();
}

void UBJS_StoreWidget::BJS_UpdateWidget()
{
	Super::BJS_UpdateWidget();

	if (auto myInstance = Cast<UBJS_GameInstance>(GetGameInstance()))
	{
		header->SetTextCash(myInstance->GetCash());

		if (Mode == 0)
		{
			for (int i = 1; i < myInstance->GetMyCharaterList().Num(); i++)
			{
				bool check = myInstance->GetMyCharaterList()[i];
				switch (i)
				{
				case 1:
					{
						CurrentCharaterWeapon(check, sbtn_1);
					}
					break;
				case 2:
					{
						CurrentCharaterWeapon(check, sbtn_2);
					}
					break;
				case 3:
					{
						CurrentCharaterWeapon(check, sbtn_3);
					}
					break;
				}
			}
		}
		else if (Mode == 1)
		{
			for (int i = 1; i < myInstance->GetMyWeaponList().Num(); i++)
			{
				bool check = myInstance->GetMyWeaponList()[i];
				switch (i)
				{
				case 1:
					{
						CurrentCharaterWeapon(check, sbtn_1);
					}
					break;
				case 2:
					{
						CurrentCharaterWeapon(check, sbtn_2);
					}
					break;
				case 3:
					{
						CurrentCharaterWeapon(check, sbtn_3);
					}
					break;
				}
			}
		}
	}
}

void UBJS_StoreWidget::BJS_PromptWidget(int32 code)
{
	Super::BJS_PromptWidget(code);

	auto instance = Cast<UBJS_GameInstance>(GetGameInstance());
	if (!instance) return;

	auto CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), instance->GetPrompt());
	if (CurrentWidget)
	{
		CurrentWidget->AddToViewport();
		auto widget = Cast<UBJS_PromptWidget>(CurrentWidget);
		if (widget)
		{
			FString str = FString(TEXT("구매 성공"));
			if (code == 0)
				str = FString(TEXT("구매 금액이 부족합니다."));
			else if (code == -1)
				str = FString(TEXT("이미 존재하는 타입의 캐릭터입니다."));
			widget->SetText(str);
		}
	}
}

void UBJS_StoreWidget::SetMode(int mode)
{
	Mode = mode;

	if (Mode == 0)
	{
		sbtn_1->SetType(1);
		sbtn_2->SetType(2);
		sbtn_3->SetType(3);
	}
	else if (Mode == 1)
	{
		sbtn_1->SetType(4);
		sbtn_2->SetType(5);
		sbtn_3->SetType(6);
	}
}

void UBJS_StoreWidget::SetStoreImage(TArray<UTexture2D*>& Array)
{
	sbtn_1->SetImageBtn(Array[0]);
	sbtn_2->SetImageBtn(Array[1]);
	sbtn_3->SetImageBtn(Array[2]);
}

void UBJS_StoreWidget::CurrentCharaterWeapon(bool IsCheck, UBJS_SelectButton_Widget* SBtn)
{
	if (IsCheck)
	{
		SBtn->SetSelectBtn(false, IsCheck);
	}
	else
	{
		SBtn->SetSelectBtn(false, IsCheck);
	}
}
