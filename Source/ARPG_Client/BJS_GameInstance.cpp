// Fill out your copyright notice in the Description page of Project Settings.


#include "BJS_GameInstance.h"

#include "BJS_CharaterState.h"
#include "ExpLvStruct.h"
#include "InventoryItem.h"
#include "ItemEquipStruct.h"
#include "ItemEtcStruct.h"
#include "PacketHandlerUtils.h"
#include "PlayerStruct.h"
#include "SkillBindStruct.h"
#include "WeaponStruct.h"
#include "SkillStruct.h"
#include "SocketSubsystem.h"
#include "Common/TcpSocketBuilder.h"
#include "Engine/DataTable.h"

UBJS_GameInstance::UBJS_GameInstance()
{
	// 1. 소켓 1개는 초기에 생성해서 메모리할당과 서버와 커넥션을 시킨다.
	// 2. 맵이 변경되면, 해당 맵의 gameMode에서 소켓을 사용할수 있는 액터를 생성한다.
	// 3. 소켓 액터 초기화시 gameInstance의 소켓주소를 받아서 연동한다.
	// 4. 데이터 수신은 gameMode의 Update에서 실행한다. 큐에 쌓아두는 방식
	// 5. 데이터 송신은 gameMode의 socket을 가져와서 send바로 때리는 방식.

	UE_LOG(LogTemp, Log, TEXT("Create BJS_GameInstance !!!"));

	MyCharaterState = MakeShared<BJS_CharaterState>();
	MyCharaterList.Init(false, 4);
	MyWeaponList.Init(false, 4);
	MyInventory = MakeShared<InventoryItem>();

	LoadBP();
	LoadDataTable();
	LoadSkill();
	LoadCharaterImg();
}

void UBJS_GameInstance::Init()
{
	Super::Init();

	if (IsConnection) return;

	const int32 port = 12129;
	const FString Address = TEXT("127.0.0.1");
	FIPv4Address::Parse(Address, Ip);

	ServerArr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	ServerArr->SetIp(Ip.Value);
	ServerArr->SetPort(port);

	SocketPtr = FTcpSocketBuilder(TEXT("MYSOCKET")).AsNonBlocking().AsReusable();
	SocketPtr->Connect(*ServerArr);
	IsConnection = true;

	WriteBuffer = MakeShared<BJS_Buffer>(1024 * 4);
}

void UBJS_GameInstance::BeginDestroy()
{
	Super::BeginDestroy();

	if (SocketPtr)
	{
		if (SocketPtr->GetConnectionState() == ESocketConnectionState::SCS_Connected)
		{
			SocketPtr->Close();
		}
		ISocketSubsystem* SocketSubsystem = ISocketSubsystem::Get();
		SocketSubsystem->DestroySocket(SocketPtr);
	}
}

void UBJS_GameInstance::FinishDestroy()
{
	Super::FinishDestroy();
}

FSocket* UBJS_GameInstance::GetSocket()
{
	return SocketPtr;
}

TSharedPtr<FInternetAddr> UBJS_GameInstance::GetServerArr()
{
	return ServerArr;
}

BJS_BufferPtr UBJS_GameInstance::GetReadBuffer()
{
	return ReadBuffer;
}

BJS_BufferPtr UBJS_GameInstance::GetWriteBuffer()
{
	return WriteBuffer;
}

TSharedPtr<BJS_CharaterState> UBJS_GameInstance::GetMyState()
{
	return MyCharaterState;
}

TArray<bool>& UBJS_GameInstance::GetMyCharaterList()
{
	return MyCharaterList;
}

TArray<bool>& UBJS_GameInstance::GetMyWeaponList()
{
	return MyWeaponList;
}

TSharedPtr<InventoryItem> UBJS_GameInstance::GetMyInventory()
{
	return MyInventory;
}

int32 UBJS_GameInstance::GetCurCharaterType() const
{
	return CurCharaterType;
}

void UBJS_GameInstance::SetCurCharaterType(int32 curCharaterType)
{
	CurCharaterType = curCharaterType;
}

int32 UBJS_GameInstance::GetCurWeaponType() const
{
	return CurWeaponType;
}

void UBJS_GameInstance::SetCurWeaponType(int32 curWeaponType)
{
	CurWeaponType = curWeaponType;
}

int32 UBJS_GameInstance::GetCash() const
{
	return Cash;
}

void UBJS_GameInstance::SetCash(int32 cash)
{
	Cash = cash;
}

void UBJS_GameInstance::LoadBP()
{
	// 캐릭터 메시 로드
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharaterMesh1(
		TEXT("/Script/Engine.SkeletalMesh'/Game/MyGame/Assets/Charater/CharaterMesh_1.CharaterMesh_1'"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharaterMesh2(
		TEXT("/Script/Engine.SkeletalMesh'/Game/MyGame/Assets/Charater/CharaterMesh_2.CharaterMesh_2'"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharaterMesh3(
		TEXT("/Script/Engine.SkeletalMesh'/Game/MyGame/Assets/Charater/CharaterMesh_3.CharaterMesh_3'"));
	check(CharaterMesh1.Succeeded());
	check(CharaterMesh2.Succeeded());
	check(CharaterMesh3.Succeeded());
	CharacterMeshList.Add(1, CharaterMesh1.Object);
	CharacterMeshList.Add(2, CharaterMesh2.Object);
	CharacterMeshList.Add(3, CharaterMesh3.Object);

	// 무기 메시 로드
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> WeaponMesh(
		TEXT("/Script/Engine.SkeletalMesh'/Game/MyGame/Assets/Weapon/WeaponsMesh/Grenade_Launcher_A.Grenade_Launcher_A'"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> WeaponMesh2(
		TEXT("/Script/Engine.SkeletalMesh'/Game/MyGame/Assets/Weapon/WeaponsMesh/Rocket_Launcher_A.Rocket_Launcher_A'"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> WeaponMesh3(
		TEXT("/Script/Engine.SkeletalMesh'/Game/MyGame/Assets/Weapon/WeaponsMesh/Shotgun_A.Shotgun_A'"));
	check(WeaponMesh.Succeeded());
	check(WeaponMesh2.Succeeded());
	check(WeaponMesh3.Succeeded());
	WeaponMeshList.Add(1, WeaponMesh.Object);
	WeaponMeshList.Add(2, WeaponMesh2.Object);
	WeaponMeshList.Add(3, WeaponMesh3.Object);

	// 캐릭터 BP 로드
	static ConstructorHelpers::FClassFinder<AActor> CharacterBP(TEXT("/Script/Engine.Blueprint'/Game/MyGame/BluePrint/BP_Charater.BP_Charater_C'"));
	check(CharacterBP.Succeeded());
	PlayerClass = CharacterBP.Class;

	// 몬스터 BP 로드
	static ConstructorHelpers::FClassFinder<AActor> MonsterBP(TEXT("/Script/Engine.Blueprint'/Game/MyGame/BluePrint/BP_Monster.BP_Monster_C'"));
	check(MonsterBP.Succeeded());
	MonsterClass = MonsterBP.Class;
}

void UBJS_GameInstance::LoadDataTable()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_Player(TEXT("/Script/Engine.DataTable'/Game/MyGame/Data/DT_Player.DT_Player'"));
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_Monster(TEXT("/Script/Engine.DataTable'/Game/MyGame/Data/DT_Monster.DT_Monster'"));
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_Weapon(TEXT("/Script/Engine.DataTable'/Game/MyGame/Data/DT_Weapon.DT_Weapon'"));
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_Skill(TEXT("/Script/Engine.DataTable'/Game/MyGame/Data/DT_Skill.DT_Skill'"));
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_SkillBind(TEXT("/Script/Engine.DataTable'/Game/MyGame/Data/DT_SkillBind.DT_SkillBind'"));
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_ExpLv(TEXT("/Script/Engine.DataTable'/Game/MyGame/Data/DT_ExpLv.DT_ExpLv'"));
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_ItemEquip(TEXT("/Script/Engine.DataTable'/Game/MyGame/Data/DT_ItemEquip.DT_ItemEquip'"));
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_ItemEtc(TEXT("/Script/Engine.DataTable'/Game/MyGame/Data/DT_ItemEtc.DT_ItemEtc'"));

	check(DT_Player.Succeeded());
	check(DT_Monster.Succeeded());
	check(DT_Weapon.Succeeded());
	check(DT_Skill.Succeeded());
	check(DT_SkillBind.Succeeded());
	check(DT_ExpLv.Succeeded());
	check(DT_ItemEquip.Succeeded());
	check(DT_ItemEtc.Succeeded());

	DT_Player.Object->GetAllRows<FPlayerStruct>(TEXT("GetAllRows"), PlayerStructs);
	DT_Monster.Object->GetAllRows<FPlayerStruct>(TEXT("GetAllRows"), MonsterStructs);
	DT_Weapon.Object->GetAllRows<FWeaponStruct>(TEXT("GetAllRows"), WeaponStructs);
	DT_Skill.Object->GetAllRows<FSkillStruct>(TEXT("GetAllRows"), SkillStructs);
	DT_SkillBind.Object->GetAllRows<FSkillBindStruct>(TEXT("GetAllRows"), SkillBindStructs);
	DT_ExpLv.Object->GetAllRows<FExpLvStruct>(TEXT("GetAllRows"), ExpLvStructs);
	DT_ItemEquip.Object->GetAllRows<FItemEquipStruct>(TEXT("GetAllRows"), ItemEquipStructs);
	DT_ItemEtc.Object->GetAllRows<FItemEtcStruct>(TEXT("GetAllRows"), ItemEtcStructs);

	for (auto skill : SkillStructs)
	{
		SkillStructMap.Add(skill->Code, skill);
	}

	for (auto skillBind : SkillBindStructs)
	{
		int32 WeaponCode = skillBind->WeaponCode;
		if (!SkillBindStructMap.Contains(WeaponCode))
			SkillBindStructMap.Add(WeaponCode);
		SkillBindStructMap[WeaponCode].Add(skillBind);
	}

	for (auto ExpLv : ExpLvStructs)
	{
		int32 Lv = ExpLv->Lv;
		ExpLvStructMap.Add(Lv, ExpLv);
	}

	for (auto item : ItemEquipStructs)
	{
		int32 Code = item->Code;	
		ItemEquipStructMap.Add(Code, item);
	}

	for (auto item : ItemEtcStructs)
	{
		int32 Code = item->Code;
		ItemEtcStructMap.Add(Code, item);
	}
	LoadSkillIcon();
	LoadItemIcon();
}

void UBJS_GameInstance::LoadSkill()
{
	ConstructorHelpers::FObjectFinder<UNiagaraSystem> Skill_101(TEXT("/Script/Niagara.NiagaraSystem'/Game/WeaponBulletVFX/NS/VFX_Laser.VFX_Laser'"));
	ConstructorHelpers::FObjectFinder<UNiagaraSystem> Skill_201(TEXT("/Script/Niagara.NiagaraSystem'/Game/WeaponBulletVFX/NS/VFX_PoisonGun.VFX_PoisonGun'"));
	ConstructorHelpers::FObjectFinder<UNiagaraSystem> Skill_301(TEXT("/Script/Niagara.NiagaraSystem'/Game/WeaponBulletVFX/NS/VFX_SmgBig.VFX_SmgBig'"));
	ConstructorHelpers::FObjectFinder<UParticleSystem> Skill_102(
		TEXT("/Script/Engine.ParticleSystem'/Game/FXVarietyPack/Particles/P_ky_magicCircle1.P_ky_magicCircle1'"));
	ConstructorHelpers::FObjectFinder<UParticleSystem> Skill_202(
		TEXT("/Script/Engine.ParticleSystem'/Game/FXVarietyPack/Particles/P_ky_healAura.P_ky_healAura'"));
	ConstructorHelpers::FObjectFinder<UParticleSystem> Skill_302(
		TEXT("/Script/Engine.ParticleSystem'/Game/FXVarietyPack/Particles/P_ky_healAura.P_ky_healAura'"));

	check(Skill_101.Succeeded());
	check(Skill_201.Succeeded());
	check(Skill_301.Succeeded());
	check(Skill_102.Succeeded());
	check(Skill_202.Succeeded());
	check(Skill_302.Succeeded());
	NiagaraSkillMap.Add(101, Skill_101.Object);
	NiagaraSkillMap.Add(201, Skill_201.Object);
	NiagaraSkillMap.Add(301, Skill_301.Object);
	ParticleSkillMap.Add(102, Skill_102.Object);
	ParticleSkillMap.Add(202, Skill_202.Object);
	ParticleSkillMap.Add(302, Skill_302.Object);

	ConstructorHelpers::FObjectFinder<UParticleSystem> FMuzzleFX(TEXT("/Script/Engine.ParticleSystem'/Game/FXVarietyPack/Particles/P_ky_hit2.P_ky_hit2'"));
	ParticleMuzzleFX = FMuzzleFX.Object;

	static ConstructorHelpers::FClassFinder<AActor> FBulletClass(TEXT("/Script/Engine.Blueprint'/Game/MyGame/BluePrint/BP_Bulet.BP_Bulet_C'"));
	static ConstructorHelpers::FClassFinder<AActor> FSkill_101Class(TEXT("/Script/Engine.Blueprint'/Game/MyGame/BluePrint/Skill/BP_Skill101.BP_Skill101_C'"));
	static ConstructorHelpers::FClassFinder<AActor> FSkill_201Class(TEXT("/Script/Engine.Blueprint'/Game/MyGame/BluePrint/Skill/BP_Skill201.BP_Skill201_C'"));
	static ConstructorHelpers::FClassFinder<AActor> FSkill_301Class(TEXT("/Script/Engine.Blueprint'/Game/MyGame/BluePrint/Skill/BP_Skill301.BP_Skill301_C'"));

	check(FBulletClass.Succeeded());
	check(FSkill_101Class.Succeeded());
	check(FSkill_201Class.Succeeded());;
	check(FSkill_301Class.Succeeded());
	SkillBulletMap.Add(0, FBulletClass.Class);
	SkillBulletMap.Add(101, FSkill_101Class.Class);
	SkillBulletMap.Add(201, FSkill_201Class.Class);
	SkillBulletMap.Add(301, FSkill_301Class.Class);

	static ConstructorHelpers::FClassFinder<AActor> FMonsterBulletClass(
		TEXT("/Script/Engine.Blueprint'/Game/MyGame/BluePrint/BP_Monster_Bulet.BP_Monster_Bulet_C'"));
	check(FMonsterBulletClass.Succeeded());
	MonsterBullet = FMonsterBulletClass.Class;

	static ConstructorHelpers::FClassFinder<AActor> FBuffSkillClass(TEXT("/Script/Engine.Blueprint'/Game/MyGame/BluePrint/BP_BuffSkill.BP_BuffSkill_C'"));
	check(FBuffSkillClass.Succeeded());
	BuffSkill = FBuffSkillClass.Class;
}

void UBJS_GameInstance::LoadSkillIcon()
{
	for (auto skill : SkillStructs)
	{
		ConstructorHelpers::FObjectFinder<UTexture2D> Img(*skill->Path.ToString());
		check(Img.Object);
		SkillIconImgMap.Add(skill->Code, Img.Object);
	}
}

void UBJS_GameInstance::LoadCharaterImg()
{
	// 이미지 셋팅
	static ConstructorHelpers::FObjectFinder<UTexture2D> CHARATER_HAED_IMAGE_1(TEXT("/Script/Engine.Texture2D'/Game/MyGame/Assets/Image/CharaterHeadPng1.CharaterHeadPng1'"));
	CharaterHeadImageMap.Add(1, CHARATER_HAED_IMAGE_1.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> CHARATER_HAED_IMAGE_2(TEXT("/Script/Engine.Texture2D'/Game/MyGame/Assets/Image/CharaterHeadPng2.CharaterHeadPng2'"));
	CharaterHeadImageMap.Add(2, CHARATER_HAED_IMAGE_2.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> CHARATER_HAED_IMAGE_3(TEXT("/Script/Engine.Texture2D'/Game/MyGame/Assets/Image/CharaterHeadPng3.CharaterHeadPng3'"));
	CharaterHeadImageMap.Add(3, CHARATER_HAED_IMAGE_3.Object);
	check(CharaterHeadImageMap.Num() == 3);
}

void UBJS_GameInstance::LoadItemIcon()
{
	for (auto item : ItemEquipStructs)
	{
		ConstructorHelpers::FObjectFinder<UTexture2D> ImgEquip(*item->Path.ToString());
		check(ImgEquip.Object);
		ItemEquipIconImgMap.Add(item->Code, ImgEquip.Object);
	}
	
	for (auto item : ItemEtcStructs)
	{
		ConstructorHelpers::FObjectFinder<UTexture2D> ImgEtc(*item->Path.ToString());
		check(ImgEtc.Object);
		ItemEtcIconImgMap.Add(item->Code, ImgEtc.Object);
	}
}

TArray<FPlayerStruct*>& UBJS_GameInstance::GetPlayerStructs()
{
	return PlayerStructs;
}

TArray<FPlayerStruct*>& UBJS_GameInstance::GetMonsterStructs()
{
	return MonsterStructs;
}

TArray<FWeaponStruct*>& UBJS_GameInstance::GetWeaponStructs()
{
	return WeaponStructs;
}

TMap<int32, TArray<FSkillBindStruct*>>& UBJS_GameInstance::GetSkillBindStructs()
{
	return SkillBindStructMap;
}

TMap<int32, FExpLvStruct*>& UBJS_GameInstance::GetExpLvStructs()
{
	return ExpLvStructMap;
}

TMap<int32, FItemEquipStruct*>& UBJS_GameInstance::GetItemEquipStructMap()
{
	return ItemEquipStructMap;
}

TMap<int32, FItemEtcStruct*>& UBJS_GameInstance::GetItemEtcStructMap()
{
	return ItemEtcStructMap;
}

TMap<int32, UTexture2D*>& UBJS_GameInstance::GetSkillIconImgMap()
{
	return SkillIconImgMap;
}

TMap<int32, UTexture2D*>& UBJS_GameInstance::GetItemEquipIconImgMap()
{
	return ItemEquipIconImgMap;
}

TMap<int32, UTexture2D*>& UBJS_GameInstance::GetItemEtcIconImgMap()
{
	return ItemEtcIconImgMap;
}

TMap<int32, UTexture2D*>& UBJS_GameInstance::GetCharaterImgMap()
{
	return CharaterHeadImageMap;
}

TMap<int32, UParticleSystem*>& UBJS_GameInstance::GetParticleSkillMap()
{
	return ParticleSkillMap;
}

UParticleSystem* UBJS_GameInstance::GetFireStartFX()
{
	return ParticleMuzzleFX;
}

TMap<int32, TSubclassOf<AActor>>& UBJS_GameInstance::GetSkillBulletMap()
{
	return SkillBulletMap;
}

TSubclassOf<AActor>& UBJS_GameInstance::GetMonsterSkillClass()
{
	return MonsterBullet;
}

TSubclassOf<AActor>& UBJS_GameInstance::GetBuffSkillClass()
{
	return BuffSkill;
}

TMap<int32, UNiagaraSystem*>& UBJS_GameInstance::GetNiagaraSkillMap()
{
	return NiagaraSkillMap;
}

TMap<int32, class USkeletalMesh*>& UBJS_GameInstance::GetCharaterMeshList()
{
	return CharacterMeshList;
}

TMap<int32, class USkeletalMesh*>& UBJS_GameInstance::GetWeaponMeshList()
{
	return WeaponMeshList;
}

TMap<int32, struct FSkillStruct*>& UBJS_GameInstance::GetSkillStructs()
{
	return SkillStructMap;
}

TSubclassOf<AActor>& UBJS_GameInstance::GetPlayerClass()
{
	return PlayerClass;
}

TSubclassOf<AActor>& UBJS_GameInstance::GetMonsterClass()
{
	return MonsterClass;
}
