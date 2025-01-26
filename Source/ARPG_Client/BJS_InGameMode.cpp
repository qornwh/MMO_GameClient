// Fill out your copyright notice in the Description page of Project Settings.


#include "BJS_InGameMode.h"

#include "BJS_CharaterState.h"
#include "BJS_FriendWidget.h"
#include "BJS_GameInstance.h"
#include "BJS_GameUI.h"
#include "BJS_InventoryWidget.h"
#include "BJS_ItemToopTip_Widget.h"
#include "BJS_MailWidget.h"
#include "BJS_Monster.h"
#include "BJS_SocketActor.h"
#include "BJS_UserWidgetBase.h"
#include "FriendSystem.h"
#include "GameClient.pb.h"
#include "PlayerStruct.h"
#include "SkillStruct.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"

ABJS_InGameMode::ABJS_InGameMode()
{
	static ConstructorHelpers::FClassFinder<UUserWidget> UI_MAIN_HUD(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/MyGame/UMG/BJS_WBP_GameUI.BJS_WBP_GameUI_C'"));
	check(UI_MAIN_HUD.Succeeded());

	MainUi = CreateWidget<UBJS_GameUI>(GetWorld(), UI_MAIN_HUD.Class);
	if (MainUi)
	{
		MainUi->AddToViewport();
		CurrentWidget = MainUi;
		IsMainUi = true;
	}
	
	static ConstructorHelpers::FClassFinder<UUserWidget> UI_INVENTORY_HUD(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/MyGame/UMG/BJS_WBP_Inventory.BJS_WBP_Inventory_C'"));
	check(UI_INVENTORY_HUD.Succeeded());
	
	InventoryUi = CreateWidget<UBJS_InventoryWidget>(GetWorld(), UI_INVENTORY_HUD.Class);
	if (InventoryUi)
	{
		InventoryUi->AddToViewport();	
		InventoryUi->SetVisibility(ESlateVisibility::Hidden);
	}
	
	static ConstructorHelpers::FClassFinder<UUserWidget> UI_FRIENDSYSTEM_HUD(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/MyGame/UMG/BJS_SWBP_Friend.BJS_SWBP_Friend_C'"));
	check(UI_FRIENDSYSTEM_HUD.Succeeded());
	
	FriendUi = CreateWidget<UBJS_FriendWidget>(GetWorld(), UI_FRIENDSYSTEM_HUD.Class);
	if (FriendUi)
	{
		FriendUi->AddToViewport();	
		FriendUi->SetVisibility(ESlateVisibility::Hidden);
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> UI_MAILBOX_HUD(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/MyGame/UMG/BJS_WBP_Mail.BJS_WBP_Mail_C'"));
	check(UI_MAILBOX_HUD.Succeeded());
	
	MailBoxUi = CreateWidget<UBJS_MailWidget>(GetWorld(), UI_MAILBOX_HUD.Class);
	if (MailBoxUi)
	{
		MailBoxUi->AddToViewport();	
		MailBoxUi->SetVisibility(ESlateVisibility::Hidden);
	}
	
	static ConstructorHelpers::FClassFinder<UUserWidget> UI_TOOLTIP_HUD(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/MyGame/UMG/BJS_SWBP_ItemToolTip.BJS_SWBP_ItemToolTip_C'"));
	check(UI_TOOLTIP_HUD.Succeeded());
	
	ItemToolTipUi = CreateWidget<UBJS_ItemToopTip_Widget>(GetWorld(), UI_TOOLTIP_HUD.Class);
	if (ItemToolTipUi)
	{
		ItemToolTipUi->AddToViewport();	
		ItemToolTipUi->SetVisibility(ESlateVisibility::Hidden);
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
		SocketActor->OnLoadDelegate.BindUObject(this, &ABJS_InGameMode::LoadGame);
		SocketActor->OnSpawnDelegate.BindUObject(this, &ABJS_InGameMode::InsertPlayer);
		SocketActor->OnDeSpawnDelegate.BindUObject(this, &ABJS_InGameMode::RemovePlayer);
		SocketActor->OnMoveStartPoint.BindUObject(this, &ABJS_InGameMode::MoveStartPoint);
		SocketActor->OnChatMessage.BindUObject(this, &ABJS_InGameMode::ReadChatMessage);
		
		protocol::CLoad pkt;
		protocol::Position* position = new protocol::Position;
		position->set_x(PlayerStart->GetActorLocation().X);
		position->set_y(PlayerStart->GetActorLocation().Y);
		position->set_z(PlayerStart->GetActorLocation().Z);
		position->set_yaw(PlayerStart->GetActorRotation().Yaw);
		pkt.set_allocated_position(position);

		SocketActor->SendMessage(pkt, protocol::MessageCode::C_LOAD);
	}
	TakeDemageList.Init(nullptr, 10);

	auto ui = Cast<UBJS_GameUI>(MainUi);
	if (ui)
	{
		ui->OnChatMessageSend.BindUObject(this, &ABJS_InGameMode::SendChatMessage);
	}
	
	auto instance = Cast<UBJS_GameInstance>(GetGameInstance());
	MyState = instance->GetMyState();
	MyInventory = instance->GetMyInventory();
	MyFriend = instance->GetMyFriend();
	MyMail = instance->GetMyMail();
	SetShowMouseCousor(false);

	// 미리 캐릭터 200개 가량 로드해둠
	for (int32 i = 0; i < 200; i++)
	{
		FVector SpawnLocation = FVector::ZeroVector;
		FRotator SpawnRotation = FRotator::ZeroRotator;
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		ABJS_Character* player = GetWorld()->SpawnActor<ABJS_Character>(instance->GetPlayerClass(), SpawnLocation, SpawnRotation, SpawnParams);;
		CustomDespawnActor<ABJS_Character>(instance->GetPlayerClass(), player);
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
}

void ABJS_InGameMode::TakeHeal(int32 SkillCode, TSharedPtr<class BJS_CharaterState> State)
{
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

	BJSCharaterStateList.Add(instance->GetMyState()->GetUUid(), instance->GetMyState());
	InventoryUi->BJS_InitWidget();
	FriendUi->BJS_InitWidget();
	MailBoxUi->BJS_InitWidget();
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
		
		auto MyFriendRef = MyFriend.Pin();
		if (MyFriendRef)
		{
			int32 friendCode = MyFriendRef->CheckFriend(State->GetName());
			MyFriendRef->UpdateFriend(friendCode, true);
		}
	}
	FriendUi->LoadFriendList();
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
		
		auto MyFriendRef = MyFriend.Pin();
		if (MyFriendRef)
		{
			int32 friendCode = MyFriendRef->CheckFriend(State->GetName());
			MyFriendRef->UpdateFriend(friendCode, true);
			FriendUi->LoadFriendList();
		}
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
	if (!IsMonster)
	{
		if (BJSCharaterStateList.Contains(UUid))
		{
			auto State = BJSCharaterStateList[UUid];
			auto Target = State->GetTarget();
			if (State->GetTarget())
			{
				GetWorld()->DestroyActor(Target);
		
				auto MyFriendRef = MyFriend.Pin();
				if (MyFriendRef)
				{
					int32 friendCode = MyFriendRef->CheckFriend(State->GetName());
					MyFriendRef->UpdateFriend(friendCode, false);
					FriendUi->LoadFriendList();
				}
			}
		}
	}
	else
	{
		if (BJSMonsterStateList.Contains(UUid))
		{
			auto State = BJSMonsterStateList[UUid];
			auto Target = State->GetTarget();
			if (State->GetTarget())
			{
				GetWorld()->DestroyActor(Target);
			}
		}
	}
}

void ABJS_InGameMode::SendChatMessage(FString Message, int32 Type)
{
	protocol::SChat pkt;
	std::string msgStr = TCHAR_TO_UTF8(*Message);
	pkt.set_text(msgStr);
	pkt.set_uuid(MyState.Pin()->GetUUid());
	pkt.set_type(Type);
	SocketActor->SendMessage(pkt, protocol::MessageCode::S_CHAT);
}

void ABJS_InGameMode::ReadChatMessage(FString Message, int32 Type, int32 Uuid)
{
	auto ui = Cast<UBJS_GameUI>(MainUi);
	if (ui)
	{
		if (BJSCharaterStateList.Contains(Uuid))
		{
			auto player = BJSCharaterStateList[Uuid];
			if (player.IsValid())
			{
				ui->ReadChatMessage(Message, Type, player->GetName());
			}
		}
		else
		{
			if (Type == 2)
			{
				ui->ReadChatMessage(Message, Type, FString{"System"});
			}
		}
	}
}

void ABJS_InGameMode::ChangeInventoryUI()
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

void ABJS_InGameMode::OpenFriendUI()
{
	if (IsMainUi)
	{
		SetShowMouseCousor(true);
		FriendUi->SetVisibility(ESlateVisibility::Visible);
		FriendUi->LoadFriendList();
	}
}

void ABJS_InGameMode::OpenToolTipUI()
{
	if (ItemToolTipUi)
	{
		SetShowMouseCousor(true);
		ItemToolTipUi->SetVisibility(ESlateVisibility::Visible);
	}
}

void ABJS_InGameMode::ChangeMailBoxUI()
{
	CurrentWidget->SetVisibility(ESlateVisibility::Hidden);
	if (IsMainUi)
	{
		CurrentWidget = MailBoxUi;
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

void ABJS_InGameMode::ClaseMyPlayer()
{
	protocol::SClosePlayer pkt;
	pkt.set_uuid(MyState.Pin()->GetUUid());
	SocketActor->SendMessage(pkt, protocol::MessageCode::S_CLOSEPLAYER);
	
	auto instance = Cast<UBJS_GameInstance>(GetGameInstance());
	if (instance)
	{
		instance->SocketDisConnect();
		instance->ResetData();
	}

	UGameplayStatics::OpenLevel(this, TEXT("LoginMap"));
}

void ABJS_InGameMode::UpdateInventoryEquipUI(int32 InvenPos, int32 State)
{
	auto ui = Cast<UBJS_InventoryWidget>(InventoryUi);
	if (ui)
	{
		if (State == 1)
		{
			// set
			ui->UpdateEquipSlot(InvenPos);
		}
		else if (State == 0)
		{
			// delete
			ui->RemoveEquipSlot(InvenPos);
		}
	}
}

void ABJS_InGameMode::UpdateEquippedItemUI(int32 EquipPos, int32 State)
{
	auto ui = Cast<UBJS_InventoryWidget>(InventoryUi);
	if (ui)
	{
		if (State == 1)
		{
			// set
			ui->UpdateEquippedSlot(EquipPos);
		}
		else if (State == 0)
		{
			// delete
			ui->UpdateEquippedSlot(EquipPos);
		}
	}
}

void ABJS_InGameMode::UpdateInventoryEtcUI(int32 EtcPos, int32 State)
{
	auto ui = Cast<UBJS_InventoryWidget>(InventoryUi);
	if (ui)
	{
		// etc는 그냥 업데이트만 한다.
		ui->UpdateEtcSlot(EtcPos);
	}
}

void ABJS_InGameMode::UpdateInventoryUI()
{
	auto ui = Cast<UBJS_InventoryWidget>(InventoryUi);
	if (ui)
	{
		ui->SetSlot();
	}
}

void ABJS_InGameMode::EquippedItemUI(int32 Invenpos, int32 Equippos)
{
	auto ui = Cast<UBJS_InventoryWidget>(InventoryUi);
	if (ui)
	{
		auto& invenItem = GetMyInventory()->GetInventoryEquipItemList()[Invenpos];
		auto& equipItem = GetMyInventory()->GetEquippedItemList()[Equippos];

		if (invenItem.IsEmpty() && equipItem.IsEmpty())
			return;

		auto tempItem = invenItem;
		invenItem = equipItem;
		equipItem = tempItem;
	}
}

void ABJS_InGameMode::SendEquippedItem(int32 Invenpos, int32 Equippos)
{
	protocol::CUpdateItems pkt;
	pkt.set_invenpos(Invenpos);
	pkt.set_equippos(Equippos);
			
	SocketActor->SendMessage(pkt, protocol::MessageCode::C_UPDATEITEMS);
}

void ABJS_InGameMode::UpdateToolTipEquipItem(EquipItem& TargetItem)
{
	if (ItemToolTipUi)
	{
		ItemToolTipUi->SetEquipItem(TargetItem);
	}
}

void ABJS_InGameMode::UpdateToolTipEtcItem(EtcItem& TargetItem)
{
	if (ItemToolTipUi)
	{
		ItemToolTipUi->SetEtcItem(TargetItem);
	}
}

void ABJS_InGameMode::UpdateMailUi()
{
	if (MailBoxUi)
	{
		MailBoxUi->SetSlot();
	}
}

void ABJS_InGameMode::SetMailEquipItem(int32 InvenPos, int32 SocketPos)
{
	if (MailBoxUi)
	{
		MailBoxUi->SetSendMailEquipItem(InvenPos, SocketPos);
	}
}

void ABJS_InGameMode::UpdateMail(int32 MailCode, int32 State)
{
	if (GetMyMail()->GetMailList().Contains(MailCode))
	{
		protocol::CUpdateMail pkt;
		protocol::Mail* mail = new protocol::Mail();
		mail->set_code(MailCode);
		pkt.set_allocated_mail(mail);
		pkt.set_type(State);
		
		SocketActor->SendMessage(pkt, protocol::MessageCode::C_UPDATEMAIL);
	}
}

void ABJS_InGameMode::AllUpdateMail(int32 State)
{
	protocol::CAllUpdateMail pkt;
	pkt.set_type(State);
		
	SocketActor->SendMessage(pkt, protocol::MessageCode::C_ALLUPDATEMAIL);
}

void ABJS_InGameMode::UpdateFriendUi()
{
	if (FriendUi)
	{
		FriendUi->LoadFriendList();
	}
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
	return MyState.Pin();
}

TSharedPtr<InventoryItem> ABJS_InGameMode::GetMyInventory()
{
	return MyInventory.Pin();
}

TSharedPtr<FriendSystem> ABJS_InGameMode::GetMyFriend()
{
	return MyFriend.Pin();
}

TSharedPtr<MailBox> ABJS_InGameMode::GetMyMail()
{
	return MyMail.Pin();
}

void ABJS_InGameMode::SellItems()
{
	protocol::CSellItems pkt;

	for (auto& itemEntry : SellEquipItems)
	{
		protocol::ItemEquip* sellItem = pkt.add_itemequips();
		sellItem->set_item_code(itemEntry.Value.ItemCode);
		sellItem->set_attack(itemEntry.Value.Attack);
		sellItem->set_speed(itemEntry.Value.Speed);
		sellItem->set_item_type(itemEntry.Value.EquipType);
		sellItem->set_equippos(itemEntry.Value.EquipPos);
		sellItem->set_invenpos(itemEntry.Value.InvenPos);
	}

	for (auto& itemEntry : SellEtcItems)
	{
		protocol::ItemEtc* sellItem = pkt.add_itemetcs();
		sellItem->set_item_code(itemEntry.Value.ItemCode);
		sellItem->set_item_count(itemEntry.Value.Count);
		sellItem->set_invenpos(itemEntry.Value.InvenPos);
		sellItem->set_item_type(itemEntry.Value.Type);
	}
	SocketActor->SendMessage(pkt, protocol::MessageCode::C_SELLITEMS);
	ResetSellItems();
}

void ABJS_InGameMode::ResetSellItems()
{
	SellEquipItems.Empty();
	SellEtcItems.Empty();
}
