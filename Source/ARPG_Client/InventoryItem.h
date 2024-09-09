// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

struct EquipItem
{
	EquipItem() : Code(0) {}
	EquipItem(int32 Code) : Code(Code) {}
	EquipItem(const EquipItem& other) : Code(other.Code) {}
	int32 Code = 0;
};

struct EtcItem
{
	EtcItem() : Code(0), Count(0) {}
	EtcItem(int32 Code, int32 Count) : Code(Code), Count(Count) {}
	EtcItem(const EtcItem& other) : Code(other.Code), Count(other.Count) {}
	int32 Code = 0;
	int32 Count = 0;
};

class ARPG_CLIENT_API InventoryItem : public TSharedFromThis<InventoryItem>
{
public:
	InventoryItem();
	~InventoryItem();

	void AddEquipItem(int32 Code);
	void AddEtcItem(int32 Code, int32 Count);
	bool UseEquipItem(int32 Code);
	bool UseEtcItem(int32 Code, int32 Count);

	void AddGold(int32 gold);
	void UseGold(int32 gold);
	void SetGold(int32 gold);

	TMap<int32, TArray<EquipItem>>& GetEquipItems() { return EquipItems; }
	TMap<int32, EtcItem>& GetEtcItems() { return EtcItems; }
	int32 GetGold() { return Gold; }

private:
	TMap<int32, TArray<EquipItem>> EquipItems;
	TMap<int32, EtcItem> EtcItems;
	int32 Gold = 0;
};
