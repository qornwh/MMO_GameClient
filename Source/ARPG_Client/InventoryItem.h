// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "google/protobuf/stubs/port.h"

enum ToolTipType
{
	NOT_DATA = 0,
	EQUIP_ITEM_TYPE = 1,
	ETC_ITEM_TYPE = 2
};

enum EquipItemType
{
	AttackItem = 1,
	SpeedItem = 2
};


struct EquipItem
{
	EquipItem(int32 itemCode, int32 equipType, int32 attack, int32 speed, int32 equipPos, int32 invenPos);
	~EquipItem();

	EquipItem& operator=(const EquipItem& other);
	bool EqualEquipItem(const EquipItem& other) const;
    bool IsEmpty();
	void SetEmptyItem();
	
	int32 ItemCode;
	int32 EquipType;
	int32 Attack;
	int32 Speed;
	int32 EquipPos;
	int32 InvenPos;
};

struct EtcItem
{
	EtcItem(int32 itemCode, int32 type, int32 count, int32 invenPos);
	~EtcItem();

	EtcItem& operator=(const EtcItem& other);
	bool EqualEtcItem(const EtcItem& other) const;

	void UpdateItem(int32 count = 1);
	void SetEmptyItem();
	bool IsEmpty();
	int32 ItemCode;
	int32 Count;
	int32 Type;
	int32 InvenPos;
};

class ARPG_CLIENT_API InventoryItem : public TSharedFromThis<InventoryItem>
{
public:
	InventoryItem();
	~InventoryItem();

	EquipItem& AddEquipItem(int32 itemCode, int32 equipType, int32 attack, int32 speed, int32 equipPos, int32 invenPos);
	EtcItem& AddEtcItem(int32 itemCode, int32 type, int32 count, int32 invenPos);
	
	bool ItemEquipped(int32 invenPos, int32 equipPos);
	void AddGold(int32 gold);
	void UseGold(int32 gold);
	void SetGold(int32 gold);
	void RemoveEquipItem(int32 Invenpos);
	void UseEtcItem(int32 Invenpos, int32 Count);
	void EquippedItem(int Invenpos, int Equippos);

	TArray<EquipItem>& GetInventoryEquipItemList() { return InventoryEquipItemList; }
	TArray<EtcItem>& GetInventoryEtcItemList() { return InventoryEtcItemList; }
	TArray<EquipItem>& GetEquippedItemList() { return EquippedItemList; }
	int32 GetGold() { return Gold; }
private:
	TArray<EquipItem> InventoryEquipItemList;
	TArray<EtcItem> InventoryEtcItemList;
	TArray<EquipItem> EquippedItemList;
	int32 Gold = 0;

	const int32 InventorySize = 20;
	const int32 EquipSize = 3;
};
