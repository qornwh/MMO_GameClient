// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BJS_SocketActor.h"
#include "Engine/GameInstance.h"
#include "Interfaces/IPv4/IPv4Address.h"
#include "BJS_GameInstance.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_CLIENT_API UBJS_GameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UBJS_GameInstance();

	virtual void Init() override;
	virtual void BeginDestroy() override;
	virtual void FinishDestroy() override;
	FSocket* GetSocket();
	TSharedPtr<FInternetAddr> GetServerArr();
	BJS_BufferPtr GetReadBuffer();
	BJS_BufferPtr GetWriteBuffer();

	TSharedPtr<class BJS_CharaterState> GetMyState();
	TArray<bool>& GetMyCharaterList();
	TArray<bool>& GetMyWeaponList();
	TSharedPtr<class InventoryItem> GetMyInventory();
	int32 GetCurCharaterType() const;
	void SetCurCharaterType(int32 curCharaterType);
	int32 GetCurWeaponType() const;
	void SetCurWeaponType(int32 curWeaponType);
	int32 GetCash() const;
	void SetCash(int32 cash);

	void LoadBP();
	void LoadDataTable();
	void LoadSkill();
	void LoadSkillIcon();
	void LoadCharaterImg();
	void LoadItemIcon();
	void LoadPromptBP();

public:
	TArray<struct FPlayerStruct*>& GetPlayerStructs();
	TArray<struct FPlayerStruct*>& GetMonsterStructs();
	TArray<struct FWeaponStruct*>& GetWeaponStructs();
	TMap<int32, struct FSkillStruct*>& GetSkillStructs();
	TMap<int32, TArray<struct FSkillBindStruct*>>& GetSkillBindStructs();
	TMap<int32, struct FExpLvStruct*>& GetExpLvStructs();
	TMap<int32, struct FItemEquipStruct*>& GetItemEquipStructMap();
	TMap<int32, struct FItemEtcStruct*>& GetItemEtcStructMap();

	TMap<int32, class UTexture2D*>& GetSkillIconImgMap();
	TMap<int32, class UTexture2D*>& GetItemEquipIconImgMap();
	TMap<int32, class UTexture2D*>& GetItemEtcIconImgMap();
	TMap<int32, class UTexture2D*>& GetCharaterImgMap();

	TMap<int32, class USkeletalMesh*>& GetCharaterMeshList();
	TMap<int32, class USkeletalMesh*>& GetWeaponMeshList();
	TMap<int32, class UParticleSystem*>& GetParticleSkillMap();
	TMap<int32, class UNiagaraSystem*>& GetNiagaraSkillMap();
	class UParticleSystem* GetFireStartFX();

	TMap<int32, TSubclassOf<class AActor>>& GetSkillBulletMap();
	TSubclassOf<class AActor>& GetMonsterSkillClass();
	TSubclassOf<class AActor>& GetBuffSkillClass();
	TSubclassOf<class AActor>& GetPlayerClass();
	TSubclassOf<class AActor>& GetMonsterClass();

	TSubclassOf<class UUserWidget>& GetPrompt();
	TSubclassOf<class UUserWidget>& GetPrompt2();

private:
	TSharedPtr<FInternetAddr> ServerArr;
	FSocket* SocketPtr;
	bool IsConnection = false;
	FIPv4Address Ip;
	BJS_BufferPtr ReadBuffer;
	BJS_BufferPtr WriteBuffer;

	// My State
	TSharedPtr<class BJS_CharaterState> MyCharaterState;
	TArray<bool> MyCharaterList;
	TArray<bool> MyWeaponList;
	int32 Cash;
	int32 CurCharaterType;
	int32 CurWeaponType;
	TSharedPtr<class InventoryItem> MyInventory;

	// Data Struct
	TArray<struct FPlayerStruct*> PlayerStructs;
	TArray<struct FPlayerStruct*> MonsterStructs;
	TArray<struct FWeaponStruct*> WeaponStructs;
	TArray<struct FSkillStruct*> SkillStructs;
	TArray<struct FSkillBindStruct*> SkillBindStructs;
	TArray<struct FExpLvStruct*> ExpLvStructs;
	TArray<struct FItemEquipStruct*> ItemEquipStructs;
	TArray<struct FItemEtcStruct*> ItemEtcStructs;
	TMap<int32, struct FSkillStruct*> SkillStructMap;
	TMap<int32, TArray<struct FSkillBindStruct*>> SkillBindStructMap;
	TMap<int32, struct FExpLvStruct*> ExpLvStructMap;
	TMap<int32, struct FItemEquipStruct*> ItemEquipStructMap;
	TMap<int32, struct FItemEtcStruct*> ItemEtcStructMap;

	// ImageIcon
	TMap<int32, class UTexture2D*> SkillIconImgMap;
	TMap<int32, class UTexture2D*> ItemEquipIconImgMap;
	TMap<int32, class UTexture2D*> ItemEtcIconImgMap;

	TMap<int32, UTexture2D*> CharaterHeadImageMap;

	// BP_C
	TSubclassOf<class AActor> PlayerClass;
	TSubclassOf<class AActor> MonsterClass;

	// Mesh
	TMap<int32, class USkeletalMesh*> CharacterMeshList;
	TMap<int32, class USkeletalMesh*> WeaponMeshList;
	TMap<int32, class USkeletalMesh*> MonsterMeshList;

	// Skill
	TMap<int32, class UParticleSystem*> ParticleSkillMap;
	TMap<int32, class UNiagaraSystem*> NiagaraSkillMap;

	// Bullet_BP
	TMap<int32, TSubclassOf<class AActor>> SkillBulletMap;
	TSubclassOf<class AActor> MonsterBullet;
	TSubclassOf<class AActor> BuffSkill;
	UParticleSystem* ParticleMuzzleFX;

	// Prompt_BP
	TSubclassOf<class UUserWidget> PromptClass; // ok type 1
	TSubclassOf<class UUserWidget> PromptClass2; // ok, cancle type2
};