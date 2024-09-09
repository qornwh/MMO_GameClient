// Fill out your copyright notice in the Description page of Project Settings.


#include "BJS_FriendSlotWidget.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"

void UBJS_FriendSlotWidget::BJS_InitWidget()
{
}

void UBJS_FriendSlotWidget::SetText(FString text)
{
	if (tb_friend)
	{
		tb_friend->SetText(FText::FromString(text));
		
		FSlateFontInfo font = tb_friend->GetFont();
		FFontOutlineSettings OutlineSettings;
		OutlineSettings.OutlineSize = 2;
		OutlineSettings.OutlineColor = FLinearColor::Black;
		font.OutlineSettings = OutlineSettings;
	}
}

void UBJS_FriendSlotWidget::SetState(int32 color)
{
	if (tb_friend)
	{
		if (color == state::whith)
		{
			tb_friend->SetColorAndOpacity(FLinearColor::White);
		}
		else if (color == state::green)
		{
			tb_friend->SetColorAndOpacity(FLinearColor::Green);
		}
	}
}

void UBJS_FriendSlotWidget::Reset()
{
	if (tb_friend)
		tb_friend->SetText(FText());
}
