// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CustomButton.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRightClick);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDoubleClick);

UCLASS()
class ARPG_CLIENT_API UCustomButton : public UUserWidget
{
	GENERATED_BODY()

public:
	// 델리게이트 프로퍼티 선언
	UPROPERTY(BlueprintAssignable, Category = "Mouse Events")
	FOnRightClick OnRightClick;

	UPROPERTY(BlueprintAssignable, Category = "Mouse Events")
	FOnDoubleClick OnDoubleClick;

protected:
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
};
