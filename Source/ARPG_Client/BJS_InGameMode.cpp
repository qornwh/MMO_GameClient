// Fill out your copyright notice in the Description page of Project Settings.


#include "BJS_InGameMode.h"

#include "BJS_CharaterState.h"
#include "BJS_GameInstance.h"
#include "BJS_GameUI.h"
#include "BJS_Monster.h"
#include "BJS_SocketActor.h"
#include "BJS_UserWidgetBase.h"
#include "EngineUtils.h"
#include "GameClient.pb.h"
#include "NiagaraSystem.h"
#include "PlayerStruct.h"
#include "SkillStruct.h"
#include "GameFramework/PlayerStart.h"

ABJS_InGameMode::ABJS_InGameMode()
{
	static ConstructorHelpers::FClassFinder<UUserWidget> UI_MAIN_HUD(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/MyGame/UMG/BJS_WBP_GameUI.BJS_WBP_GameUI_C'"));
	check(UI_MAIN_HUD.Succeeded());

	CurrentWidget = CreateWidget<UBJS_UserWidgetBase>(GetWorld(), UI_MAIN_HUD.Class);
	if (CurrentWidget)
	{
		CurrentWidget->AddToViewport();
		MainUi = CurrentWidget;
		IsMainUi = true;
	}
	
	static ConstructorHelpers::FClassFinder<UUserWidget> UI_INVENTORY_HUD(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/MyGame/UMG/BJS_WBP_Inventory.BJS_WBP_Inventory_C'"));
	check(UI_INVENTORY_HUD.Succeeded());
	
	InventoryUi = CreateWidget<UBJS_UserWidgetBase>(GetWorld(), UI_INVENTORY_HUD.Class);
	if (InventoryUi)
	{
		InventoryUi->AddToViewport();	
		InventoryUi->SetVisibility(ESlateVisibility::Hidden);
	}
	
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 0.1;
}

void ABJS_InGameMode::StartPlay()
{
	Super::StartPlay();
}

void ABJS_InGameMode::BeginPlay()
{
	Super::BeginPlay();

	SocketActor = CustomSpawnActor<ABJS_SocketActor>(SocketClass);
	if (PlayerStart)
	{
		protocol::CLoad pkt;
		protocol::Position* position = new protocol::Position;
		position->set_x(PlayerStart->GetActorLocation().X);
		position->set_y(PlayerStart->GetActorLocation().Y);
		position->set_z(PlayerStart->GetActorLocation().Z);
		position->set_yaw(PlayerStart->GetActorRotation().Yaw);
		pkt.set_allocated_position(position);

		SocketActor->SendMessage(pkt, protocol::MessageCode::C_LOAD);
		SocketActor->OnLoadDelegate.BindUObject(this, &ABJS_InGameMode::LoadGame);
		SocketActor->OnSpawnDelegate.BindUObject(this, &ABJS_InGameMode::InsertPlayer);
		SocketActor->OnDeSpawnDelegate.BindUObject(this, &ABJS_InGameMode::RemovePlayer);
		SocketActor->OnMoveStartPoint.BindUObject(this, &ABJS_InGameMode::MoveStartPoint);
		SocketActor->OnChatMessage.BindUObject(this, &ABJS_InGameMode::ReadChatMessage);
	}
	TakeDemageList.Init(nullptr, 10);

	auto ui = Cast<UBJS_GameUI>(MainUi);
	if (ui)
	{
		ui->OnChatMessageSend.BindUObject(this, &ABJS_InGameMode::SendChatMessage);
	}
}

void ABJS_InGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	CurrentWidget->BJS_UpdateWidget();
}

void ABJS_InGameMode::SetTakeDemageList(TSharedPtr<class BJS_CharaterState> State)
{
	TakeDemageList[TakeDemageCnt] = State;
	TakeDemageCnt++;
}

void ABJS_InGameMode::TakeDemage(bool isMonster, int32 skillCode, TSharedPtr<class BJS_CharaterState> State)
{
	protocol::SUnitDemage pkt;
	auto instance = Cast<UBJS_GameInstance>(GetGameInstance());
	pkt.set_uuid(State->GetUUid());
	pkt.set_is_monster(isMonster);
	if (!State)
	{
		TakeDemageCnt = 0;
		return;
	}
	for (int32 i = 0; i < TakeDemageCnt; i++)
	{
		auto targetState = TakeDemageList[i];
		if (!targetState.Pin())
			continue;

		auto curState = targetState.Pin();
		if (isMonster)
		{
			int32 attack = State->GetAttack();
			// 레벨 1당 데미지 5퍼 증가
			int32 lv = State->GetLv();
			double skillAttackPer = 1.0f;
			if (instance->GetSkillStructs().Contains(skillCode))
			{
				skillAttackPer = instance->GetSkillStructs()[skillCode]->Value / 100.f;
			}
			// 소수점 버림
			int32 demageValue = attack * skillAttackPer * (1 + (lv - 1) * 0.05);
			protocol::Demage* demage = pkt.add_demage();
			demage->set_uuid(curState->GetUUid());
			demage->set_is_monster(true);
			demage->set_demage(demageValue);
			protocol::Position* position = new protocol::Position();
			position->set_x(curState->GetX());
			position->set_y(curState->GetY());
			position->set_yaw(curState->GetYaw());
			demage->set_allocated_position(position);

			curState->GetTarget()->TakeDemage(demageValue, State->GetTarget());
		}
		else
		{
			// 해당 경우 내가 맞음
			int32 attack = State->GetAttack();
			int32 lv = State->GetLv();
			double skillAttackPer = 1.0f;
			int32 demageValue = attack * skillAttackPer * (1 + (lv - 1) * 0.05);

			protocol::Demage* demage = pkt.add_demage();
			demage->set_uuid(State->GetUUid());
			demage->set_is_monster(false);
			demage->set_demage(demageValue);

			curState->GetTarget()->TakeDemage(demageValue, State->GetTarget());
		}
	}
	TakeDemageCnt = 0;
	SocketActor->SendMessage(pkt, protocol::MessageCode::S_UNITDEMAGE);
}

void ABJS_InGameMode::TakeHeal(int32 SkillCode, TSharedPtr<class BJS_CharaterState> State)
{
	auto instance = Cast<UBJS_GameInstance>(GetGameInstance());
	if (instance)
	{
		int32 heal = 0;
		if (instance->GetSkillStructs().Contains(SkillCode))
		{
			heal = instance->GetSkillStructs()[SkillCode]->Value;
		}
		
		protocol::SUnitDemage pkt;
		pkt.set_uuid(State->GetUUid());
		protocol::Demage* demage = pkt.add_demage();
		demage->set_uuid(State->GetUUid());
		demage->set_is_monster(false);
		demage->set_demage(heal);
		demage->set_is_heal(true);
		SocketActor->SendMessage(pkt, protocol::MessageCode::S_UNITDEMAGE);
		State->GetTarget()->TakeHeal(heal);
	}
}

void ABJS_InGameMode::LoadGame()
{
	auto instance = Cast<UBJS_GameInstance>(GetGameInstance());
	check(instance);
	check(CurrentWidget);

	auto myState = instance->GetMyState();
	{
		auto gameUi = Cast<UBJS_GameUI>(CurrentWidget);
		gameUi->SetName(myState->GetName());
		gameUi->SetLv(FString::FromInt(myState->GetLv()));
		gameUi->SetHp(static_cast<float>(myState->GetHp() / myState->GetMaxHp()));
		gameUi->SetExp(myState->GetHp());
		gameUi->BJS_InitWidget();
	}
	InventoryUi->BJS_InitWidget();

	BJSCharaterStateList.Add(instance->GetMyState()->GetUUid(), instance->GetMyState());
	MyState = instance->GetMyState();
	MyInventory = instance->GetMyInventory();

	for (auto& entry : BJSMonsterStateList)
	{
		auto State = entry.Value;
		int32 Code = State->GetCode();

		const auto MonsterStruct = instance->GetMonsterStructs()[Code];
		check(MonsterStruct);

		State->SetAttack(MonsterStruct->Attack);
		State->SetMaxHp(MonsterStruct->Hp);
		State->SetSpeed(MonsterStruct->MoveSpeed);
		State->SetName(MonsterStruct->Name.ToString());

		FVector SpawnLocation = FVector(State->GetX(), State->GetY(), 100.f);
		FRotator SpawnRotation = FRotator(0.f, State->GetYaw(), 0.f);
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		ABJS_Monster* monster = CustomSpawnActor<ABJS_Monster>(instance->GetMonsterClass(), SpawnLocation, SpawnRotation);
		monster->SetState(State);
	}

	for (auto& entry : BJSCharaterStateList)
	{
		auto State = entry.Value;
		if (State->GetUUid() == instance->GetMyState()->GetUUid())
			continue;
		int32 Code = State->GetCode();

		const auto PlayerStruct = instance->GetPlayerStructs()[Code];
		check(PlayerStruct);

		State->SetAttack(PlayerStruct->Attack);
		State->SetMaxHp(PlayerStruct->Hp);
		State->SetSpeed(PlayerStruct->MoveSpeed);

		FVector SpawnLocation = FVector(State->GetX(), State->GetY(), 100.f);
		FRotator SpawnRotation = FRotator(0.f, State->GetYaw(), 0.f);
		check(instance->GetPlayerClass());
		ABJS_Character* player = CustomSpawnActor<ABJS_Character>(instance->GetPlayerClass(), SpawnLocation, SpawnRotation);
		check(player);
		player->SetState(State);
		player->LoadInfo(Code, State->GetWeaponCode());
	}
}

void ABJS_InGameMode::InsertPlayer(bool IsMonster, int32 UUid)
{
	auto instance = Cast<UBJS_GameInstance>(GetGameInstance());
	check(instance);

	if (!IsMonster)
	{
		auto State = BJSCharaterStateList[UUid];
		int32 Code = State->GetCode();

		auto PlayerStruct = instance->GetPlayerStructs()[Code];
		check(PlayerStruct);

		State->SetAttack(PlayerStruct->Attack);
		State->SetMaxHp(PlayerStruct->Hp);
		State->SetSpeed(PlayerStruct->MoveSpeed);

		FVector SpawnLocation = FVector(State->GetX(), State->GetY(), 100.f);
		FRotator SpawnRotation = FRotator(0.f, State->GetYaw(), 0.f);
		ABJS_Character* Player = CustomSpawnActor<ABJS_Character>(instance->GetPlayerClass(), SpawnLocation, SpawnRotation);
		Player->SetState(State);
		Player->LoadInfo(Code, State->GetWeaponCode());
	}
	else
	{
		auto State = BJSMonsterStateList[UUid];
		int32 Code = State->GetCode();

		const auto MonsterStruct = instance->GetMonsterStructs()[Code];
		check(MonsterStruct);

		State->SetAttack(MonsterStruct->Attack);
		State->SetMaxHp(MonsterStruct->Hp);
		State->SetSpeed(MonsterStruct->MoveSpeed);
		State->SetName(MonsterStruct->Name.ToString());

		FVector SpawnLocation = FVector(State->GetX(), State->GetY(), 120.f);
		FRotator SpawnRotation = FRotator(0.f, State->GetYaw(), 0.f);
		ABJS_Monster* Player = CustomSpawnActor<ABJS_Monster>(instance->GetMonsterClass(), SpawnLocation, SpawnRotation);
		Player->SetState(State);
	}
}

void ABJS_InGameMode::RemovePlayer(bool IsMonster, int32 UUid)
{
	if (BJSCharaterStateList.Contains(UUid))
	{
		auto State = BJSCharaterStateList[UUid];
		auto Target = State->GetTarget();
		if (State->GetTarget())
		{
			GetWorld()->DestroyActor(Target);
		}
	}
}

void ABJS_InGameMode::SendChatMessage(FString Message, int32 Type)
{
	protocol::SChat pkt;
	std::string msgStr = TCHAR_TO_UTF8(*Message);
	pkt.set_text(msgStr);
	pkt.set_uuid(MyState->GetUUid());
	pkt.set_type(Type);
	SocketActor->SendMessage(pkt, protocol::MessageCode::S_CHAT);
}

void ABJS_InGameMode::ReadChatMessage(FString Message, int32 Type, int32 Uuid)
{
	auto ui = Cast<UBJS_GameUI>(MainUi);
	if (ui)
	{
		auto player = BJSCharaterStateList[Uuid];
		if (player.IsValid())
		{
			ui->ReadChatMessage(Message, Type, player->GetName());
		}
	}
}

void ABJS_InGameMode::ChangeInventoryIU()
{
	CurrentWidget->SetVisibility(ESlateVisibility::Hidden);
	if (IsMainUi)
	{
		CurrentWidget = InventoryUi;
		SetShowMouseCousor(true);
		IsMainUi = false;
	}
	else
	{
		CurrentWidget = MainUi;
		SetShowMouseCousor(false);
		IsMainUi = true;
	}
	CurrentWidget->SetVisibility(ESlateVisibility::Visible);
}

void ABJS_InGameMode::DestroyPlayer(bool IsMonster, int32 UUid)
{
	auto instance = Cast<UBJS_GameInstance>(GetGameInstance());
	check(instance);

	if (IsMonster)
	{
		auto State = BJSMonsterStateList[UUid];
		if (State->GetTarget())
			State->GetTarget()->Destroy();
	}
}

TMap<int32, TSharedPtr<BJS_CharaterState>>& ABJS_InGameMode::GetCharaterStateList()
{
	return BJSCharaterStateList;
}

TMap<int32, TSharedPtr<BJS_CharaterState>>& ABJS_InGameMode::GetMonsterStateList()
{
	return BJSMonsterStateList;
}

TSharedPtr<BJS_CharaterState> ABJS_InGameMode::GetMyState()
{
	return MyState;
}

TSharedPtr<InventoryItem> ABJS_InGameMode::GetMyInventory()
{
	return MyInventory;
}

void ABJS_InGameMode::SellItems()
{
	protocol::CSellItems pkt;

	for (auto& itemEntry : SellEquipItems)
	{
		for (auto& item : itemEntry.Value)
		{
			protocol::ItemEquip* sellItem = pkt.add_itemequips();
			sellItem->set_item_code(item.Code);
		}
	}

	for (auto& itemEntry : SellEtcItems)
	{
		auto& item = itemEntry.Value;
		protocol::ItemEtc* sellItem = pkt.add_itemetcs();
		sellItem->set_item_code(item.Code);
		sellItem->set_item_count(item.Count);
	}
	SocketActor->SendMessage(pkt, protocol::MessageCode::C_SELLITEMS);
	ResetSellItems();
}

void ABJS_InGameMode::ResetSellItems()
{
	SellEquipItems.Empty();
	SellEtcItems.Empty();
}
