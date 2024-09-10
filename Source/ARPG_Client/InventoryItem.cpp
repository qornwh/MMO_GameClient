// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryItem.h"

EquipItem::EquipItem(int32 uniqueId, int32 itemCode, int32 equipType, int32 attack, int32 speed, int32 isEquip, int32 use):
	UniqueId(uniqueId),
	ItemCode(itemCode),
	EquipType(equipType),
	Attack(attack),
	Speed(speed),
	IsEquip(isEquip),
	Use(use)
{
}

EquipItem::~EquipItem()
{
}

EquipItem& EquipItem::operator=(const EquipItem& other)
{
	UniqueId = other.UniqueId;
	ItemCode = other.ItemCode;
	EquipType = other.EquipType;
	Attack = other.Attack;
	Speed = other.Speed;
	IsEquip = other.IsEquip;
	Use = other.Use;
	return *this;
}

void EquipItem::UpdateItem(int32 use)
{
    Use = use;
}

EtcItem::EtcItem(int32 itemCode, int32 type, int32 count): ItemCode(itemCode), Count(count), Type(type)
{
}

EtcItem::~EtcItem()
{
}

EtcItem& EtcItem::operator=(const EtcItem& other)
{
	ItemCode = other.ItemCode;
	Count = other.Count;
	Type = other.Type;
	return *this;
}

void EtcItem::UpdateItem(int32 count)
{
    Count = count;
}

InventoryItem::InventoryItem() : Gold(0)
{
}

InventoryItem::~InventoryItem()
{
}

EquipItem& InventoryItem::AddEquipItem(int32 uniqueId, int32 itemCode, int32 equipType, int32 attack, int32 speed, int32 isEquip, int32 use)
{
	EquipItem Equip{uniqueId, itemCode, equipType, attack, speed, isEquip, use};
	EquipItems.Add(Equip.UniqueId, Equip);
	return EquipItems[Equip.UniqueId];
}

EtcItem& InventoryItem::AddEtcItem(int32 itemCode, int32 type, int32 count)
{
	auto it = EtcItems.Find(itemCode);
	if (EtcItems.Contains(itemCode))
	{
		it->UpdateItem(it->Count);
	}
	else
	{
		EtcItem etc{itemCode, type, count};
		EtcItems.Add(itemCode, etc);
	}
	return *EtcItems.Find(itemCode);
}

EquipItem& InventoryItem::AddEquipItem(EquipItem& Equip)
{
	EquipItems.Add(Equip.UniqueId, Equip);
	return Equip;
}

EtcItem& InventoryItem::AddEtcItem(EtcItem& Etc)
{
	int32 itemCode = Etc.ItemCode;
	auto it = EtcItems.Find(itemCode);
	if (EtcItems.Contains(itemCode))
	{
		it->UpdateItem(it->Count + Etc.Count);
	}
	else
	{
		EtcItems.Add(itemCode, Etc);
	}
	return Etc;
}

bool InventoryItem::UseEquipItem(int32 UniqueId)
{
	if (!EquipItems.Contains(UniqueId))
	{
		return false;
	}

	EquipItems.Remove(UniqueId);
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
