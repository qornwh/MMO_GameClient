// Fill out your copyright notice in the Description page of Project Settings.


#include "BJS_SkillWidget.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"

void UBJS_SkillWidget::SetSkillIcon(UTexture2D* Image)
{
	FSlateBrush Brush;
	Brush.SetResourceObject(Image);
	Brush.ImageSize = FVector2D(Image->GetSizeX(), Image->GetSizeY());
	img_icon->SetBrush(Brush);
}

void UBJS_SkillWidget::SetUpdateCoolTime(float CoolTime)
{
	FText time = FText::FromString(FString::Printf(TEXT("%2.2f"), CoolTime));
	tb_time->SetText(time);
	if (CoolTime == 0.f)
	{
		img_icon->SetOpacity(1);
	}
	else
	{
		img_icon->SetOpacity(0.5);
	}
}
