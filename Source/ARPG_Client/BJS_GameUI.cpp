// Fill out your copyright notice in the Description page of Project Settings.


#include "BJS_GameUI.h"

#include "BJS_CharaterState.h"
#include "BJS_GameInstance.h"
#include "BJS_GameModeBase.h"
#include "BJS_InGameMode.h"
#include "BJS_SkillManagerWidget.h"
#include "ExpLvStruct.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"

UBJS_GameUI::UBJS_GameUI()
{
}

void UBJS_GameUI::BJS_InitWidget()
{
	Super::BJS_InitWidget();
	SetImg();
	skill_manager->BJS_InitWidget();
	btn_chat->OnClicked.AddDynamic(this, &UBJS_GameUI::SendChatMessage);
	auto mode = Cast<ABJS_InGameMode>(GetWorld()->GetAuthGameMode());
	if (mode)
	{
		btn_mail->OnClicked.AddDynamic(mode, &ABJS_InGameMode::ChangeMailBoxUI);
		btn_iventory->OnClicked.AddDynamic(mode, &ABJS_InGameMode::ChangeInventoryUI);
		btn_friends->OnClicked.AddDynamic(mode, &ABJS_InGameMode::OpenFriendUI);
		btn_logout->OnClicked.AddDynamic(mode, &ABJS_InGameMode::ClaseMyPlayer);
	}
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

void UBJS_GameUI::ReadChatMessage(const FString& Message, int32 Type, const FString& Name)
{
	UTextBlock* TextBox = NewObject<UTextBlock>(mtb_chatlog);
		
	if (TextBox)
	{
		mtb_chatlog->AddChild(TextBox);
		FString Msg = FString::Printf(TEXT("%s : %s"), *Name, *Message);
		TextBox->SetText(FText::FromString(Msg));

		if (Type == 1)
		{
			TextBox->SetColorAndOpacity(FLinearColor::White);
		}
		else if (Type == 2)
		{
			TextBox->SetColorAndOpacity(FLinearColor::Yellow);
		}

		FSlateFontInfo font = TextBox->GetFont();
		FFontOutlineSettings OutlineSettings;
		OutlineSettings.OutlineSize = 2;
		OutlineSettings.OutlineColor = FLinearColor::Black;
		font.OutlineSettings = OutlineSettings;
		
		font.Size = 30;
		TextBox->SetFont(font);
	}
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

void UBJS_GameUI::SendChatMessage()
{
	OnChatMessageSend.Execute(tb_chat->GetText().ToString(), 1);
	tb_chat->SetText(FText());
}
