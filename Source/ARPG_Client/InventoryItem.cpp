// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryItem.h"

InventoryItem::InventoryItem() : Gold(0)
{
}

InventoryItem::~InventoryItem()
{
}

void InventoryItem::AddEquipItem(int32 Code)
{
	if (!EquipItems.Contains(Code))
	{
		EquipItems.Add(Code, TArray<EquipItem>());
	}
	EquipItems.Find(Code)->Add(EquipItem(Code));
}

void InventoryItem::AddEtcItem(int32 Code, int32 Count)
{
	if (!EtcItems.Contains(Code))
	{
		EtcItems.Add(Code, EtcItem(Code, Count));
	}
	else
	{
		EtcItems.Find(Code)->Count += Count;
	}
}

bool InventoryItem::UseEquipItem(int32 Code)
{
	if (!EquipItems.Contains(Code))
	{
		return false;
	}
	
	if (EquipItems.Find(Code)->Num() > 0)
	{
		EquipItems.Find(Code)->Pop();
		return true;
	}
	return false;
}

bool InventoryItem::UseEtcItem(int32 Code, int32 Count)
{
	if (!EtcItems.Contains(Code))
	{
		return false;
	}
	
	if (EtcItems.Find(Code)->Count >= Count)
	{
		EtcItems.Find(Code)->Count -= Count;
		if (EtcItems.Find(Code)->Count == 0)
		{
			EtcItems.Remove(Code);
		}
		return true;
	}
	return false;
}

void InventoryItem::AddGold(int32 gold)
{
	Gold += gold;
}

void InventoryItem::UseGold(int32 gold)
{
	Gold -= gold;
}

void InventoryItem::SetGold(int32 gold)
{
	Gold = gold;
}
