// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BJS_GameModeBase.h"
#include "InventoryItem.h"
#include "BJS_InGameMode.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_CLIENT_API ABJS_InGameMode : public ABJS_GameModeBase
{
private:
	GENERATED_BODY()

public:
	ABJS_InGameMode();

	virtual void StartPlay() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	
	UFUNCTION()
	void LoadGame();
	UFUNCTION()
	void InsertPlayer(bool IsMonster, int32 UUid);
	UFUNCTION()
	void RemovePlayer(bool IsMonster, int32 UUid);
	UFUNCTION()
	void SendChatMessage(FString Message, int32 Type);
	UFUNCTION()
	void ReadChatMessage(FString Message, int32 Type, int32 Uuid);
	UFUNCTION()
	void ChangeInventoryUI();
	UFUNCTION()
	void OpenFriendUI();
	UFUNCTION()
	void OpenToolTipUI();
	UFUNCTION()
	void ClaseMyPlayer();

	void SetTakeDemageList(TSharedPtr<class BJS_CharaterState> State);
	void TakeDemage(bool IsMonster, int32 SkillCode, TSharedPtr<class BJS_CharaterState> State);
	void TakeHeal(int32 SkillCode, TSharedPtr<class BJS_CharaterState> State);
	void DestroyPlayer(bool IsMonster, int32 UUid);
	void UpdateMyFriendUI(int32 FriendCode, int32 State);
	void UpdateInventoryEquipUI(int32 EquipUnipeId, int32 State);
	void UpdateInventoryEtcUI(int32 EtcItemCode, int32 State);
	void UpdateInventoryUI();
	void SendEquippedItem(const EquipItem& TargetItem);
	void UpdateToolTipEquipItem(EquipItem& TargetItem);
	void UpdateToolTipEtcItem(EtcItem& TargetItem);
	
	TMap<int32, TSharedPtr<class BJS_CharaterState>>& GetCharaterStateList();
	TMap<int32, TSharedPtr<class BJS_CharaterState>>& GetMonsterStateList();
	TSharedPtr<class BJS_CharaterState> GetMyState();
	TSharedPtr<class InventoryItem> GetMyInventory();
	TSharedPtr<class FriendSystem> GetMyFriend();
	
	UFUNCTION()
	void SellItems();
	UFUNCTION()
	void ResetSellItems();

public:
	// 일단 임시로 판매리스트 열어둔다
	TMap<int32, EquipItem> SellEquipItems;
	TMap<int32, EtcItem> SellEtcItems;

private:
	TMap<int32, TSharedPtr<class BJS_CharaterState>> BJSCharaterStateList;
	TMap<int32, TSharedPtr<class BJS_CharaterState>> BJSMonsterStateList;
	TSharedPtr<class BJS_CharaterState> MyState;
	TWeakPtr<class InventoryItem> MyInventory;
	TWeakPtr<class FriendSystem> MyFriend;

	int32 TakeDemageCnt = 0;
	TArray<TWeakPtr<class BJS_CharaterState>> TakeDemageList;

	bool IsMainUi = true;
	TObjectPtr<class UBJS_UserWidgetBase> MainUi;
	TObjectPtr<class UBJS_UserWidgetBase> InventoryUi;
	TObjectPtr<class UBJS_FriendWidget> FriendUi;
	TObjectPtr<class UBJS_ItemToopTip_Widget> ItemToolTipUi;
};
