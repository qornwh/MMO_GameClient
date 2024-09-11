// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomButton.h"

FReply UCustomButton::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
	{
		// 오른쪽 클릭 델리게이트 호출
		OnRightClick.Broadcast();
		return FReply::Handled();
	}

	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

FReply UCustomButton::NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		// 더블 클릭 델리게이트 호출
		OnDoubleClick.Broadcast();
		return FReply::Handled();
	}
	
	return Super::NativeOnMouseButtonDoubleClick(InGeometry, InMouseEvent);
}
