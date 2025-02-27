// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "ItemDragDropOperation.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_CLIENT_API UItemDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()
public:
	UUserWidget* GetWidgetReference() const;
	void SetWidgetReference(UUserWidget* Widget);

private:
	UUserWidget* WidgetReference;
};
