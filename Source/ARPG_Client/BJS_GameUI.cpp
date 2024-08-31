// Fill out your copyright notice in the Description page of Project Settings.


#include "BJS_GameUI.h"

#include "BJS_CharaterState.h"
#include "BJS_GameInstance.h"
#include "BJS_GameModeBase.h"
#include "BJS_InGameMode.h"
#include "BJS_SkillManagerWidget.h"
#include "ExpLvStruct.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

UBJS_GameUI::UBJS_GameUI()
{
}

void UBJS_GameUI::BJS_InitWidget()
{
	Super::BJS_InitWidget();
	SetImg();
	skill_manager->BJS_InitWidget();
}

void UBJS_GameUI::SetImg()
{
	auto instance = Cast<UBJS_GameInstance>(GetGameInstance());
	if (instance)
	{
		int32 Code = instance->GetCurCharaterType();
		if (instance->GetCharaterImgMap().Contains(Code))
		{
			auto Image = instance->GetCharaterImgMap()[Code];
			if (Image)
			{
				FSlateBrush Brush;
				Brush.SetResourceObject(Image);
				Brush.ImageSize = FVector2D(Image->GetSizeX(), Image->GetSizeY());
				img_charater->SetBrush(Brush);
			}
		}
	}
}

void UBJS_GameUI::SetName(FString Name)
{
	tb_Name->SetText(FText::FromString(Name));
}

void UBJS_GameUI::SetLv(FString Lv)
{
	tb_lv->SetText(FText::FromString("LV " + Lv));
}

void UBJS_GameUI::SetExp(float Exp)
{
	pb_exp->SetPercent(Exp);
}

void UBJS_GameUI::SetHp(float Hp)
{
	pb_hp->SetPercent(Hp);
}

void UBJS_GameUI::BJS_UpdateWidget()
{
	Super::BJS_UpdateWidget();

	auto mode = Cast<ABJS_InGameMode>(GetWorld()->GetAuthGameMode());
	auto instance = Cast<UBJS_GameInstance>(GetGameInstance());
	if (mode && instance)
	{
		auto myState = mode->GetMyState();
		if (!myState.IsValid()) return;

		skill_manager->BJS_SubUpdateWidget();
		SetHp(StaticCast<float>(myState->GetHp()) / myState->GetMaxHp());
		SetLv(FString::FromInt(myState->GetLv()));

		if (instance->GetExpLvStructs().Find(myState->GetLv()))
		{
			SetExp(StaticCast<float>(myState->GetExp()) / instance->GetExpLvStructs()[myState->GetLv()]->Exp);
		}
	}
}

void UBJS_GameUI::CoolTimeUpdate(int32 KeyBind, float CoolTime)
{
	if (skill_manager)
	{
		skill_manager->CoolTimeUpdate(KeyBind, CoolTime);
	}
}
