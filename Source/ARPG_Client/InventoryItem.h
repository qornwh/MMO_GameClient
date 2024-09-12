// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

struct EquipItem
{
	EquipItem(int32 uniqueId, int32 itemCode, int32 equipType, int32 attack, int32 speed, int32 isEquip, int32 position, int32 use);
	~EquipItem();

	EquipItem& operator=(const EquipItem& other);

	void UpdateItem(int32 use = 1);
	void SetEmptyItem();
	int32 UniqueId;
	int32 ItemCode;
	int32 EquipType;
	int32 Attack;
	int32 Speed;
	int32 IsEquip;
	int32 Position;
	int32 Use;
};

struct EtcItem
{
	EtcItem(int32 itemCode, int32 type, int32 count, int32 position);
	~EtcItem();

	EtcItem& operator=(const EtcItem& other);

	void UpdateItem(int32 count = 1);
	void SetEmptyItem();
	int32 ItemCode;
	int32 Count;
	int32 Type;
	int32 Position;
};

class ARPG_CLIENT_API InventoryItem : public TSharedFromThis<InventoryItem>
{
public:
	InventoryItem();
	~InventoryItem();

	EquipItem& AddEquipItem(int32 uniqueId, int32 itemCode, int32 equipType, int32 attack, int32 speed, int32 isEquip, int32 position, int32 use);
	EtcItem& AddEtcItem(int32 itemCode, int32 type, int32 count, int32 position);
	EquipItem& AddEquipItem(EquipItem& Equip);
	EtcItem& AddEtcItem(EtcItem& Etc);
	bool UseEquipItem(int32 UniqueId);
	bool ItemEquipped(int32 UniqueId, int32 Equipped, int32 Position);
	bool UseEtcItem(int32 Code, int32 Count);

	void AddGold(int32 gold);
	void UseGold(int32 gold);
	void SetGold(int32 gold);

	TMap<int32, EquipItem>& GetEquipItems() { return EquipItems; }
	TMap<int32, EtcItem>& GetEtcItems() { return EtcItems; }
	int32 GetGold() { return Gold; }

private:
	TMap<int32, EquipItem> EquipItems;
	TMap<int32, EtcItem> EtcItems;
	int32 Gold = 0;
};
