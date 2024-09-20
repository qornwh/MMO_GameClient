// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryItem.h"

EquipItem::EquipItem(int32 uniqueId, int32 itemCode, int32 equipType, int32 attack, int32 speed, int32 isEquip, int32 position, int32 use):
	UniqueId(uniqueId),
	ItemCode(itemCode),
	EquipType(equipType),
	Attack(attack),
	Speed(speed),
	IsEquip(isEquip),
	Position(position),
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

void EquipItem::SetEmptyItem()
{
	UniqueId = -1;
	ItemCode = -1;
	EquipType = -1;
	Attack = -1;
	Speed = -1;
	IsEquip = 0;
	Use = -1;
}

EtcItem::EtcItem(int32 itemCode, int32 type, int32 count, int32 position): ItemCode(itemCode), Count(count), Type(type), Position(position)
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

void EtcItem::SetEmptyItem()
{
	ItemCode = -1;
	Count = -1;
	Type = -1;
}

InventoryItem::InventoryItem() : Gold(0)
{
}

InventoryItem::~InventoryItem()
{
}

EquipItem& InventoryItem::AddEquipItem(int32 uniqueId, int32 itemCode, int32 equipType, int32 attack, int32 speed, int32 isEquip, int32 position, int32 use)
{
	EquipItem Equip{uniqueId, itemCode, equipType, attack, speed, isEquip, position, use};
	EquipItems.Add(Equip.UniqueId, Equip);
	return EquipItems[Equip.UniqueId];
}

EtcItem& InventoryItem::AddEtcItem(int32 itemCode, int32 type, int32 count, int32 position)
{
	auto it = EtcItems.Find(itemCode);
	if (EtcItems.Contains(itemCode))
	{
		it->UpdateItem(count);
	}
	else
	{
		EtcItem etc{itemCode, type, count, position};
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
	return true;
}

bool InventoryItem::ItemEquipped(int32 UniqueId, int32 Equipped, int32 Position)
{
	if (!EquipItems.Contains(UniqueId))
	{
		return false;
	}
	EquipItems.Find(UniqueId)->IsEquip = Equipped;
	EquipItems.Find(UniqueId)->Position = Position;
	return true;
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
