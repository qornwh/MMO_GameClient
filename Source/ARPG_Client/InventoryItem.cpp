// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryItem.h"

EquipItem::EquipItem(int32 itemCode, int32 equipType, int32 attack, int32 speed, int32 equipPos, int32 invenPos):
	ItemCode(itemCode),
	EquipType(equipType),
	Attack(attack),
	Speed(speed),
	EquipPos(equipPos),
	InvenPos(invenPos)
{
}

EquipItem::~EquipItem()
{
}

EquipItem& EquipItem::operator=(const EquipItem& other)
{
	ItemCode = other.ItemCode;
	EquipType = other.EquipType;
	Attack = other.Attack;
	Speed = other.Speed;
	EquipPos = other.EquipPos;
	InvenPos = other.InvenPos;
	return *this;
}

bool EquipItem::EqualEquipItem(const EquipItem& other) const
{
	if (ItemCode == other.ItemCode && EquipType == other.EquipType && Attack == other.Attack && Speed == other.Speed)
		return true;
	return false;
}

bool EquipItem::IsEmpty()
{
	if (ItemCode == -1)
		return true;
	return false;
}

void EquipItem::SetEmptyItem()
{
	ItemCode = -1;
	EquipType = -1;
	Attack = -1;
	Speed = -1;
	EquipPos = -1;
	InvenPos = -1;
}

EtcItem::EtcItem(int32 itemCode, int32 type, int32 count, int32 invenPos): ItemCode(itemCode), Count(count), Type(type), InvenPos(invenPos)
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
	InvenPos = other.InvenPos;
	return *this;
}

bool EtcItem::EqualEtcItem(const EtcItem& other) const
{
	if (ItemCode == other.ItemCode
		&& Count == other.Count
		&& Type == other.Type)
	{
		return true;
	}
	return false;
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

bool EtcItem::IsEmpty()
{
	if (ItemCode < 0 || Count <= 0)
	{
		return true;
	}
	return false;
}

InventoryItem::InventoryItem() : Gold(0)
{
	InventoryEquipItemList.Init(EquipItem{-1,-1,-1,-1,-1,-1}, InventorySize);
	InventoryEtcItemList.Init(EtcItem{-1,-1,-1,-1}, InventorySize);
	EquippedItemList.Init(EquipItem{-1,-1,-1,-1,-1,-1}, EquipSize);
}

InventoryItem::~InventoryItem()
{
}

EquipItem& InventoryItem::AddEquipItem(int32 itemCode, int32 equipType, int32 attack, int32 speed, int32 equipPos, int32 invenPos)
{
	EquipItem Equip{itemCode, equipType, attack, speed, equipPos, invenPos};
	if (invenPos >= 0)
	{
		InventoryEquipItemList[Equip.InvenPos] = Equip;
		return InventoryEquipItemList[Equip.InvenPos];
	}
	else
	{
		EquippedItemList[Equip.EquipPos] = Equip;
		return EquippedItemList[Equip.EquipPos];
	}
}

EtcItem& InventoryItem::AddEtcItem(int32 itemCode, int32 type, int32 count, int32 invenPos)
{
	EtcItem etc{itemCode, type, count, invenPos};
	InventoryEtcItemList[invenPos] = etc;
	return InventoryEtcItemList[invenPos];
}

bool InventoryItem::ItemEquipped(int32 invenPos, int32 equipPos)
{
	if (invenPos >= 0 && equipPos >= 0)
	{
		EquipItem& invenItem = InventoryEquipItemList[invenPos];
		EquipItem& equippedItem = EquippedItemList[equipPos];

		if (invenItem.IsEmpty() && equippedItem.IsEmpty())
			return false;

		EquipItem& temp = invenItem;
		invenItem = equippedItem;
		equippedItem = temp;
		
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

void InventoryItem::RemoveEquipItem(int32 Invenpos)
{
	InventoryEquipItemList[Invenpos].SetEmptyItem();
}

void InventoryItem::UseEtcItem(int32 Invenpos, int32 Count)
{
	if (!InventoryEtcItemList[Invenpos].IsEmpty())
	{
		InventoryEtcItemList[Invenpos].Count -= Count;
		if (InventoryEtcItemList[Invenpos].Count <= 0)
			InventoryEtcItemList[Invenpos].SetEmptyItem();
	}
}

void InventoryItem::EquippedItem(int Invenpos, int Equippos)
{
	EquipItem& invenItem = InventoryEquipItemList[Invenpos];
	EquipItem& equipItem = EquippedItemList[Equippos];

	EquipItem tempItem = invenItem;
	invenItem = equipItem;
	equipItem = tempItem;
	invenItem.InvenPos = Invenpos;
	invenItem.EquipPos = -1;
	equipItem.InvenPos = -1;
	equipItem.EquipPos = Equippos;
}
