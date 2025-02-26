// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemDragDropOperation.h"

UUserWidget* UItemDragDropOperation::GetWidgetReference() const
{
	return WidgetReference;
}

void UItemDragDropOperation::SetWidgetReference(UUserWidget* WidgetReference)
{
	this->WidgetReference = WidgetReference;
}
