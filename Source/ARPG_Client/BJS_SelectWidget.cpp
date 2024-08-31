// Fill out your copyright notice in the Description page of Project Settings.


#include "BJS_SelectWidget.h"

#include "BJS_GameInstance.h"
#include "BJS_SelectButton_Widget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UBJS_SelectWidget::NativeConstruct()
{
	Super::NativeConstruct();

	btn_back->OnClicked.AddDynamic(this, &UBJS_SelectWidget::OnBackEvent);
	
	sbtn_c1->GetBtn()->OnClicked.AddDynamic(sbtn_c1, &UBJS_SelectButton_Widget::OnSelectBtn);
	sbtn_c1->SetType(1);
	sbtn_c2->GetBtn()->OnClicked.AddDynamic(sbtn_c2, &UBJS_SelectButton_Widget::OnSelectBtn);
	sbtn_c2->SetType(2);
	sbtn_c3->GetBtn()->OnClicked.AddDynamic(sbtn_c3, &UBJS_SelectButton_Widget::OnSelectBtn);
	sbtn_c3->SetType(3);

	sbtn_w1->GetBtn()->OnClicked.AddDynamic(sbtn_w1, &UBJS_SelectButton_Widget::OnSelectBtn);
	sbtn_w1->SetType(4);
	sbtn_w2->GetBtn()->OnClicked.AddDynamic(sbtn_w2, &UBJS_SelectButton_Widget::OnSelectBtn);
	sbtn_w2->SetType(5);
	sbtn_w3->GetBtn()->OnClicked.AddDynamic(sbtn_w3, &UBJS_SelectButton_Widget::OnSelectBtn);
	sbtn_w3->SetType(6);
}

UBJS_SelectWidget::UBJS_SelectWidget()
{
	
}

void UBJS_SelectWidget::OnSelectEvent()
{
	OnSelect.Execute();
}

void UBJS_SelectWidget::OnBackEvent()
{
	OnBack.Execute();
}

void UBJS_SelectWidget::BJS_UpdateWidget()
{
	Super::BJS_UpdateWidget();

	if (auto myInstance = Cast<UBJS_GameInstance>(GetGameInstance()))
	{
		for (int i = 1; i < myInstance->GetMyCharaterList().Num(); i++)
		{
			int curIdx = myInstance->GetCurCharaterType();
			bool check = myInstance->GetMyCharaterList()[i];
			switch (i)
			{
			case 1:
                {
					CurrentCharaterWeapon(check, i, curIdx, sbtn_c1);
                }
				break;
			case 2:
				{
					CurrentCharaterWeapon(check, i, curIdx, sbtn_c2);
				}
				break;
			case 3:
				{
					CurrentCharaterWeapon(check, i, curIdx, sbtn_c3);
				}
				break;
			}
		}
		for (int i = 1; i < myInstance->GetMyWeaponList().Num(); i++)
		{
			int curIdx = myInstance->GetCurWeaponType();
			bool check = myInstance->GetMyWeaponList()[i];
			switch (i)
			{
			case 1:
				{
					CurrentCharaterWeapon(check, i, curIdx, sbtn_w1);
				}
				break;
			case 2:
				{
					CurrentCharaterWeapon(check, i, curIdx, sbtn_w2);
				}
				break;
			case 3:
				{
					CurrentCharaterWeapon(check, i, curIdx, sbtn_w3);
				}
				break;
			}
		}
	}
}

void UBJS_SelectWidget::SetSelectImage(TArray<UTexture2D*>& CharaterArray, TArray<UTexture2D*>& WeaponArray)
{
	sbtn_c1->SetImageBtn(CharaterArray[0]);
	sbtn_c2->SetImageBtn(CharaterArray[1]);
	sbtn_c3->SetImageBtn(CharaterArray[2]);
	
	sbtn_w1->SetImageBtn(WeaponArray[0]);
	sbtn_w2->SetImageBtn(WeaponArray[1]);
	sbtn_w3->SetImageBtn(WeaponArray[2]);
}

void UBJS_SelectWidget::UpdateCurrentType()
{
	
}

void UBJS_SelectWidget::CurrentCharaterWeapon(bool IsCheck, int32 Idx, int32 CurIdx, UBJS_SelectButton_Widget* SBtn)
{
	if (IsCheck)
	{
		if (Idx == CurIdx)
			SBtn->SetSelectBtn(true, IsCheck);
		else
			SBtn->SetSelectBtn(false, IsCheck);
	}
	else
	{
		SBtn->SetSelectBtn(false, IsCheck);
	}
}
