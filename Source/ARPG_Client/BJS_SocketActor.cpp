// Fill out your copyright notice in the Description page of Project Settings.


#include "BJS_SocketActor.h"

#include "BJS_Character.h"
#include "BJS_CharaterState.h"
#include "BJS_GameInstance.h"
#include "BJS_InGameMode.h"
#include "BJS_UserWidgetBase.h"
#include "FriendSystem.h"
#include "GameClient.pb.h"
#include "InventoryItem.h"
#include "MailBox.h"
#include "PacketHandlerUtils.h"
#include "PlayerStruct.h"
#include "Sockets.h"

ABJS_SocketActor::ABJS_SocketActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ABJS_SocketActor::BeginPlay()
{
	Super::BeginPlay();

	auto myInstance = Cast<UBJS_GameInstance>(GetGameInstance());
	if (myInstance)
	{
		if (!myInstance->GetIsConnect())
			myInstance->SocketConnect();
		
		MySocket = myInstance->GetSocket();
		check(MySocket != nullptr);

		ReadBuffer = myInstance->GetReadBuffer();
		if (ReadBuffer == nullptr)
			ReadBuffer = MakeShared<BJS_Buffer>();
	}
	
	WriteBuffer = MakeShared<BJS_Buffer>(1024 * 4);
}

void ABJS_SocketActor::HandlePacket(BYTE* Buffer, PacketHeader* Header)
{
	uint16 id = Header->GetId();

	switch (id)
	{
	case protocol::MessageCode::LOGINACCESS:
		{
			LoginAccessHandler(Buffer, Header, static_cast<int32>(sizeof(PacketHeader)));
		}
		break;
	case protocol::MessageCode::S_CURRENTINFO:
		{
			CurrentInfoHandler(Buffer, Header, static_cast<int32>(sizeof(PacketHeader)));
		}
		break;
	case protocol::MessageCode::S_BUYRESULT:
		{
			BuyResultHandler(Buffer, Header, static_cast<int32>(sizeof(PacketHeader)));
		}
		break;
	case protocol::MessageCode::S_PLAYERDATA:
		{
			PlayerDataHandler(Buffer, Header, static_cast<int32>(sizeof(PacketHeader)));
		}
		break;
	case protocol::MessageCode::S_LOAD:
		{
			LoadHandler(Buffer, Header, static_cast<int32>(sizeof(PacketHeader)));
		}
		break;
	case protocol::MessageCode::S_INSERTPLAYER:
		{
			InsertPlayerHandler(Buffer, Header, static_cast<int32>(sizeof(PacketHeader)));
		}
		break;
	case protocol::MessageCode::S_UNITSTATES:
		{
			CharaterUpdateHandler(Buffer, Header, static_cast<int32>(sizeof(PacketHeader)));
		}
		break;
	case protocol::MessageCode::S_MOVE:
		{
			MoveHandler(Buffer, Header, static_cast<int32>(sizeof(PacketHeader)));
		}
		break;
	case protocol::MessageCode::C_ATTACK:
		{
			AttackHandler(Buffer, Header, static_cast<int32>(sizeof(PacketHeader)));
		}
		break;
	case protocol::MessageCode::S_UNITDEMAGE:
		{
			CharaterDemageHandler(Buffer, Header, static_cast<int32>(sizeof(PacketHeader)));
		}
		break;
	case protocol::MessageCode::C_PLAYERAIM:
		{
			PlayerAimHandler(Buffer, Header, static_cast<int32>(sizeof(PacketHeader)));
		}
		break;
	case protocol::MessageCode::C_PLAYERJUMP:
		{
			PlayerJumpHandler(Buffer, Header, static_cast<int32>(sizeof(PacketHeader)));
		}
		break;
	case protocol::MessageCode::S_EXPLV:
		{
			ExpLvHandler(Buffer, Header, static_cast<int32>(sizeof(PacketHeader)));
		}
		break;
	case protocol::MessageCode::UPDATEINVENTROY:
		{
			UpdateInventoryHandler(Buffer, Header, static_cast<int32>(sizeof(PacketHeader)));
		}
		break;
	case protocol::MessageCode::S_LOADINVENTORY:
		{
			LoadInventoryHandler(Buffer, Header, static_cast<int32>(sizeof(PacketHeader)));
		}
		break;
	case protocol::MessageCode::C_SELLITEMS:
		{
			SellItemsHandler(Buffer, Header, static_cast<int32>(sizeof(PacketHeader)));
		}
		break;
	case protocol::MessageCode::S_CHAT:
		{
			ChatHandler(Buffer, Header, static_cast<int32>(sizeof(PacketHeader)));
		}
		break;
	case protocol::MessageCode::S_CLOSEPLAYER:
		{
			ClosePlayerHandler(Buffer, Header, static_cast<int32>(sizeof(PacketHeader)));
		}
		break;
	case protocol::MessageCode::S_FRIENDSYSTEM:
		{
			FriendSystemHandler(Buffer, Header, static_cast<int32>(sizeof(PacketHeader)));
		}
		break;
	case protocol::MessageCode::C_UPDATEITEMS:
		{
			UpdateItemsHandler(Buffer, Header, static_cast<int32>(sizeof(PacketHeader)));
		}
		break;
	case protocol::MessageCode::C_LOADMAIL:
		{
			LoadMailHandler(Buffer, Header, static_cast<int32>(sizeof(PacketHeader)));
		}
		break;
	case protocol::MessageCode::C_UPDATEMAIL:
		{
			UpdateMailHandler(Buffer, Header, static_cast<int32>(sizeof(PacketHeader)));
		}
		break;
	case protocol::MessageCode::S_SENDMAIL:
		{
			UpdateSendMailHandler(Buffer, Header, static_cast<int32>(sizeof(PacketHeader)));
		}
		break;
	}
}

void ABJS_SocketActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (MySocket == nullptr)
	{
		ReConnectSocket();
		return;
	}
	if (!MySocket->Wait(ESocketWaitConditions::WaitForRead, FTimespan(1))) return;

	int32 len = 0;
	MySocket->Recv(ReadBuffer->WritePos(), ReadBuffer->FreeSize(), len);

	if (len == 0) return;
	if (!ReadBuffer->OnWrite(len)) return;

	int32 dataSize = ReadBuffer->DataSize();
	BYTE* buffer = ReadBuffer->ReadPos();
	int32 processLen = 0;
	while (true)
	{
		PacketHeader* header = reinterpret_cast<PacketHeader*>(&buffer[processLen]);
		if (!PacketHandlerUtils::CheckPacketHeader(header, processLen, len))
			break;

		HandlePacket(&buffer[processLen], header);
		processLen += header->GetSize();
	}

	if (!ReadBuffer->OnRead(processLen) || processLen <= 0 || dataSize < processLen)
		return;
	ReadBuffer->Clean();
}

void ABJS_SocketActor::SendMessage(google::protobuf::Message& Pkt, uint16 Id)
{
	PacketHandlerUtils::MakePacketHandler(WriteBuffer, Pkt, Id);
	int32 byteSize = 0;
	MySocket->Send(WriteBuffer->ReadPos(), WriteBuffer->DataSize(), byteSize);
	WriteBuffer->OnRead(byteSize);
	WriteBuffer->Clean();
}

ESocketConnectionState ABJS_SocketActor::GetConnectionState()
{
	if (MySocket)
		MySocket->GetConnectionState();
	return SCS_NotConnected;
}

void ABJS_SocketActor::Close()
{
	if (MySocket)
	{
		MySocket->Close();
	}
}

void ABJS_SocketActor::ReConnectSocket()
{
	if (GetGameInstance())
	{
		auto myInstance = Cast<UBJS_GameInstance>(GetGameInstance());

		if (myInstance)
		{
			if (myInstance->GetIsConnect())
			{
				MySocket = myInstance->GetSocket();

				MySocket->Connect(*myInstance->GetServerArr());
				check(MySocket != nullptr);
			}
		}
	}
}

void ABJS_SocketActor::LoginAccessHandler(BYTE* Buffer, PacketHeader* Header, int32 Offset)
{
	protocol::LoginAccess pkt;
	if (PacketHandlerUtils::ParsePacketHandler(pkt, Buffer, Header->GetSize() - Offset, Offset))
	{
		if (pkt.result() == 1)
		{
			if (auto MyInstance = Cast<UBJS_GameInstance>(GetGameInstance()))
			{
				int32 curCharaterType = pkt.curcharatertype();
				int32 curWeaponType = pkt.curweapontype();
				int32 cash = pkt.cash();
				MyInstance->SetCash(cash);
				MyInstance->SetCurCharaterType(curCharaterType);
				MyInstance->SetCurWeaponType(curWeaponType);
				for (auto& charater : pkt.charater())
				{
					int32 code = charater.code();
					MyInstance->GetMyCharaterList()[code] = true;
					if (code == curCharaterType)
					{
						TCHAR* nameArr = UTF8_TO_TCHAR(charater.name().c_str());
						MyInstance->GetMyState()->SetLv(charater.lv());
						MyInstance->GetMyState()->SetName(nameArr);
					}
				}

				int32 i = 1;
				for (auto& weapon : pkt.weaponlist())
				{
					if (weapon == 1)
						MyInstance->GetMyWeaponList()[i] = true;
					i++;
				}
			}
		}
		OnLoginAccess.Execute(pkt.result());
	}
}

void ABJS_SocketActor::CurrentInfoHandler(BYTE* Buffer, PacketHeader* Header, int32 Offset)
{
	protocol::SCurrentInfo pkt;
	if (PacketHandlerUtils::ParsePacketHandler(pkt, Buffer, Header->GetSize() - Offset, Offset))
	{
		int32 cash = pkt.cash();
		int32 charatertype = pkt.charatertype();
		int32 weapontype = pkt.weapontype();

		if (auto MyInstance = Cast<UBJS_GameInstance>(GetGameInstance()))
		{
			MyInstance->SetCash(cash);
			MyInstance->SetCurWeaponType(weapontype);
			if (charatertype != MyInstance->GetCurCharaterType())
			{
				MyInstance->SetCurCharaterType(charatertype);

				auto charater = pkt.charater();
				TCHAR* nameArr = UTF8_TO_TCHAR(charater.name().c_str());
				MyInstance->GetMyState()->SetLv(charater.lv());
				MyInstance->GetMyState()->SetName(nameArr);
				MyInstance->GetMyState()->SetExp(pkt.exp());
			}

			auto* currentWidget = Cast<ABJS_GameModeBase>(GetWorld()->GetAuthGameMode())->GetCurrentWidget();
			if (currentWidget)
			{
				currentWidget->BJS_UpdateWidget();
			}
		}
	}
}

void ABJS_SocketActor::BuyResultHandler(BYTE* Buffer, PacketHeader* Header, int32 Offset)
{
	protocol::SBuyResult pkt;
	if (PacketHandlerUtils::ParsePacketHandler(pkt, Buffer, Header->GetSize() - Offset, Offset))
	{
		int32 cash = pkt.cash();

		if (auto MyInstance = Cast<UBJS_GameInstance>(GetGameInstance()))
		{
			if (pkt.result() > 0)
			{
				MyInstance->SetCash(cash);

				for (auto& charater : pkt.charater())
				{
					int32 code = charater.code();
					MyInstance->GetMyCharaterList()[code] = true;
				}

				int32 i = 1;
				for (auto& weapon : pkt.weaponlist())
				{
					if (weapon == 1)
						MyInstance->GetMyWeaponList()[i] = true;
					i++;
				}
			}

			auto* currentWidget = Cast<ABJS_GameModeBase>(GetWorld()->GetAuthGameMode())->GetCurrentWidget();
			if (currentWidget)
			{
				currentWidget->BJS_UpdateWidget();
				currentWidget->BJS_PromptWidget(pkt.result());
			}
		}
	}
}

void ABJS_SocketActor::PlayerDataHandler(BYTE* Buffer, PacketHeader* Header, int32 Offset)
{
	protocol::SPlayerData pkt;
	if (PacketHandlerUtils::ParsePacketHandler(pkt, Buffer, Header->GetSize() - Offset, Offset))
	{
		auto myInstance = Cast<UBJS_GameInstance>(GetGameInstance());
		if (myInstance)
		{
			auto player = pkt.player();
			auto state = myInstance->GetMyState();
			FString name = UTF8_TO_TCHAR(player.name().c_str());
			int32 code = player.code();
			int32 uuid = player.uuid();
			int32 hp = player.hp();
			int32 lv = player.lv();
			state->SetName(name);
			state->SetUUid(uuid);
			state->SetCode(code);
			state->SetHp(hp);
			state->SetLv(lv);
			state->SetExp(pkt.exp());
			auto data = myInstance->GetPlayerStructs()[code];
			state->SetAttack(data->Attack);
			state->SetMaxHp(data->Hp);
			state->SetSpeed(data->MoveSpeed);
		}
	}
}

void ABJS_SocketActor::LoadHandler(BYTE* Buffer, PacketHeader* Header, int32 Offset)
{
	protocol::SLoad pkt;
	if (PacketHandlerUtils::ParsePacketHandler(pkt, Buffer, Header->GetSize() - Offset, Offset))
	{
		auto mode = Cast<ABJS_InGameMode>(GetWorld()->GetAuthGameMode());
		check(mode != nullptr);
		for (auto unitState : pkt.unit())
		{
			// state 생성
			TSharedPtr<BJS_CharaterState> StatePtr = MakeShared<BJS_CharaterState>();
			auto& unit = unitState.unit();
			StatePtr->SetHp(unit.hp());
			StatePtr->SetLv(unit.lv());
			StatePtr->SetUUid(unit.uuid());
			StatePtr->SetCode(unit.code());
			StatePtr->SetState(unit.state());
			StatePtr->SetX(unit.position().x());
			StatePtr->SetY(unit.position().y());
			StatePtr->SetYaw(unit.position().yaw());
			if (!unitState.is_monster())
			{
				StatePtr->SetIsMonster(false);
				mode->GetCharaterStateList().Add(unit.uuid(), StatePtr);
				StatePtr->SetWeaponCode(unit.weaponcode());
				StatePtr->SetName(UTF8_TO_TCHAR(unit.name().c_str()));
			}
			else
			{
				StatePtr->SetIsMonster(true);
				mode->GetMonsterStateList().Add(unit.uuid(), StatePtr);
			}
		}
		OnLoadDelegate.Execute();
	}
}

void ABJS_SocketActor::InsertPlayerHandler(BYTE* Buffer, PacketHeader* Header, int32 Offset)
{
	protocol::SInsertplayer pkt;
	if (PacketHandlerUtils::ParsePacketHandler(pkt, Buffer, Header->GetSize() - Offset, Offset))
	{
		auto mode = Cast<ABJS_InGameMode>(GetWorld()->GetAuthGameMode());
		check(mode != nullptr);

		auto& unit = pkt.player();
		if (!mode->GetCharaterStateList().Contains(unit.uuid()))
		{
			TSharedPtr<BJS_CharaterState> StatePtr = MakeShared<BJS_CharaterState>();
			StatePtr->SetHp(unit.hp());
			StatePtr->SetLv(unit.lv());
			StatePtr->SetUUid(unit.uuid());
			StatePtr->SetName(UTF8_TO_TCHAR(unit.name().c_str()));
			StatePtr->SetCode(unit.code());
			StatePtr->SetWeaponCode(unit.weaponcode());
			StatePtr->SetState(unit.state());
			StatePtr->SetX(unit.position().x());
			StatePtr->SetY(unit.position().y());
			StatePtr->SetYaw(unit.position().yaw());
			mode->GetCharaterStateList().Add(unit.uuid(), StatePtr);
			OnSpawnDelegate.Execute(false, unit.uuid());
		}
	}
}

void ABJS_SocketActor::CharaterUpdateHandler(BYTE* Buffer, PacketHeader* Header, int32 Offset)
{
	protocol::SUnitStates pkt;
	if (PacketHandlerUtils::ParsePacketHandler(pkt, Buffer, Header->GetSize() - Offset, Offset))
	{
		auto mode = Cast<ABJS_InGameMode>(GetWorld()->GetAuthGameMode());
		check(mode);

		auto MonsterState = mode->GetMonsterStateList();
		auto PlayerState = mode->GetCharaterStateList();
		for (auto& state : pkt.unit_state())
		{
			auto& unit = state.unit();
			int32 uuid = unit.uuid();
			if (state.is_monster())
			{
				if (MonsterState.Contains(uuid))
				{
					int32 unitState = unit.state();
					MonsterState[uuid]->SetState(unitState);
					if (unit.has_position())
					{
						auto& pos = unit.position();
						MonsterState[uuid]->SetX(pos.x());
						MonsterState[uuid]->SetY(pos.y());
						MonsterState[uuid]->SetYaw(pos.yaw());
					}

					switch (unitState)
					{
					case BJS_CharaterState::CharaterState::IDLE:
						{
							MonsterState[uuid]->SetHp(unit.hp());
							// MonsterState[uuid]->GetTarget()->SetHidden(true);
							// 로드된거 제거되고 재생성이라 lv, name, type등 고정된거는 패스한다
							OnSpawnDelegate.Execute(true, unit.uuid());
						}
						break;
					case BJS_CharaterState::CharaterState::ATTACK:
						{
							if (!state.has_attack()) break;
							auto& attack = state.attack();
							MonsterState[uuid]->GetTarget()->SetTarget(attack.target_uuid());
							MonsterState[uuid]->GetTarget()->Attack();
						}
						break;
					case BJS_CharaterState::CharaterState::HITED:
						{
							MonsterState[uuid]->SetHp(unit.hp());
						}
						break;
					case BJS_CharaterState::CharaterState::DIE:
						{
							MonsterState[uuid]->SetHp(0);
							// MonsterState[uuid]->GetTarget()->SetHidden(true);
							OnDeSpawnDelegate.Execute(true, unit.uuid());
						}
						break;
					default:
						break;
					}
				}
			}
			else
			{
				if (PlayerState.Contains(uuid))
				{
					int32 unitState = unit.state();
					switch (unitState)
					{
					case BJS_CharaterState::CharaterState::HITED:
						{
							PlayerState[uuid]->SetHp(unit.hp());
						}
						break;
					case BJS_CharaterState::CharaterState::DIE:
						{
							// 일단 시작지점으로 이동시킨다.
							OnMoveStartPoint.Execute(PlayerState[uuid]->GetTarget());
							PlayerState[uuid]->SetHp(unit.hp());
						}
						break;
					case BJS_CharaterState::CharaterState::HEAL:
						{
							PlayerState[uuid]->SetHp(unit.hp());
						}
						break;
					default:
						break;
					}
				}
			}
		}
	}
}

void ABJS_SocketActor::MoveHandler(BYTE* Buffer, PacketHeader* Header, int32 Offset)
{
	protocol::SMove pkt;
	if (PacketHandlerUtils::ParsePacketHandler(pkt, Buffer, Header->GetSize() - Offset, Offset))
	{
		auto mode = Cast<ABJS_InGameMode>(GetWorld()->GetAuthGameMode());
		if (mode)
		{
			if (mode->GetCharaterStateList().Contains(pkt.uuid()))
			{
				auto state = mode->GetCharaterStateList()[pkt.uuid()];
				state->SetX(pkt.position().x());
				state->SetY(pkt.position().y());
				state->SetYaw(pkt.position().yaw());
			}
		}
	}
}

void ABJS_SocketActor::AttackHandler(BYTE* Buffer, PacketHeader* Header, int32 Offset)
{
	protocol::CAttack pkt;
	if (PacketHandlerUtils::ParsePacketHandler(pkt, Buffer, Header->GetSize() - Offset, Offset))
	{
		UE_LOG(LogTemp, Log, TEXT("ATTACK %d : %d!!!"), pkt.uuid(), pkt.skill_code());
		auto mode = Cast<ABJS_InGameMode>(GetWorld()->GetAuthGameMode());
		check(mode);

		int32 uuid = pkt.uuid();
		auto& PlayerState = mode->GetCharaterStateList();
		if (PlayerState.Contains(uuid) && PlayerState[uuid]->GetTarget())
		{
			PlayerState[uuid]->GetTarget()->PlayMotion(pkt.skill_code(), true);
		}
	}
}

void ABJS_SocketActor::CharaterDemageHandler(BYTE* Buffer, PacketHeader* Header, int32 Offset)
{
	protocol::SUnitDemage pkt;
	if (PacketHandlerUtils::ParsePacketHandler(pkt, Buffer, Header->GetSize() - Offset, Offset))
	{
		int32 uuid = pkt.uuid();
		auto mode = Cast<ABJS_InGameMode>(GetWorld()->GetAuthGameMode());
		check(mode);
		auto& MonsterState = mode->GetMonsterStateList();
		auto& PlayerState = mode->GetCharaterStateList();

		for (auto& demage : pkt.demage())
		{
			bool isMonster = demage.is_monster();
			int32 targetUUid = demage.uuid();
			int32 demageVal = demage.demage();
			bool hasPos = demage.has_position();

			if (isMonster)
			{
				if (MonsterState.Contains(targetUUid) && MonsterState[targetUUid]->GetTarget())
				{
					MonsterState[targetUUid]->GetTarget()->TakeDemage(demageVal);
					if (hasPos)
					{
						FVector loc = MonsterState[targetUUid]->GetTarget()->GetActorLocation();
						loc.X = demage.position().x();
						loc.Y = demage.position().y();
						MonsterState[targetUUid]->GetTarget()->SetActorLocation(loc);
					}
				}
			}
			else
			{
				if (PlayerState.Contains(targetUUid) && PlayerState[targetUUid]->GetTarget())
				{
					if (demage.is_heal())
					{
						PlayerState[targetUUid]->GetTarget()->TakeHeal(demageVal);
					}
					else
					{
						PlayerState[targetUUid]->GetTarget()->TakeDemage(demageVal);
					}
				}
			}
		}
	}
}

void ABJS_SocketActor::PlayerAimHandler(BYTE* Buffer, PacketHeader* Header, int32 Offset)
{
	protocol::CPlayerAim pkt;
	if (PacketHandlerUtils::ParsePacketHandler(pkt, Buffer, Header->GetSize() - Offset, Offset))
	{
		auto mode = Cast<ABJS_InGameMode>(GetWorld()->GetAuthGameMode());
		if (mode)
		{
			if (mode->GetCharaterStateList().Contains(pkt.uuid()))
			{
				auto state = mode->GetCharaterStateList()[pkt.uuid()];
				state->GetTarget()->SetAim(pkt.isaim());
				state->GetTarget()->ChangeWeaponSocket();
			}
		}
	}
}

void ABJS_SocketActor::PlayerJumpHandler(BYTE* Buffer, PacketHeader* Header, int32 Offset)
{
	protocol::CPlayerJump pkt;
	if (PacketHandlerUtils::ParsePacketHandler(pkt, Buffer, Header->GetSize() - Offset, Offset))
	{
		auto mode = Cast<ABJS_InGameMode>(GetWorld()->GetAuthGameMode());
		if (mode)
		{
			if (mode->GetCharaterStateList().Contains(pkt.uuid()))
			{
				auto state = mode->GetCharaterStateList()[pkt.uuid()];
				if (pkt.isjump())
				{
					state->GetTarget()->StartJump();
				}
				else
				{
					state->GetTarget()->StopJump();
				}
			}
		}
	}
}

void ABJS_SocketActor::ExpLvHandler(BYTE* Buffer, PacketHeader* Header, int32 Offset)
{
	protocol::SExpLv pkt;
	if (PacketHandlerUtils::ParsePacketHandler(pkt, Buffer, Header->GetSize() - Offset, Offset))
	{
		auto mode = Cast<ABJS_InGameMode>(GetWorld()->GetAuthGameMode());
		if (mode)
		{
			mode->GetMyState()->SetLv(pkt.lv());
			mode->GetMyState()->SetExp(pkt.exp());
		}
	}
}

void ABJS_SocketActor::UpdateInventoryHandler(BYTE* Buffer, PacketHeader* Header, int32 Offset)
{
	protocol::UpdateInventory pkt;
	if (PacketHandlerUtils::ParsePacketHandler(pkt, Buffer, Header->GetSize() - Offset, Offset))
	{
		auto mode = Cast<ABJS_InGameMode>(GetWorld()->GetAuthGameMode());
		if (mode)
		{
			for (auto& item : pkt.itemequips())
			{
				mode->GetMyInventory()->AddEquipItem(
					item.unipeid(),
					item.item_code(),
					item.item_type(),
					item.attack(),
					item.speed(),
					item.is_equip(),
					item.position(),
					0);
				mode->UpdateInventoryEquipUI(item.unipeid(), 1);
			}

			for (auto& item : pkt.itemetcs())
			{
				mode->GetMyInventory()->AddEtcItem(item.item_code(), item.item_type(), item.item_count(), item.position());
				mode->UpdateInventoryEtcUI(item.item_code(), 1);
			}
			mode->GetMyInventory()->SetGold(pkt.gold());
			mode->UpdateInventoryUI();
		}
	}
}

void ABJS_SocketActor::LoadInventoryHandler(BYTE* Buffer, PacketHeader* Header, int32 Offset)
{
	protocol::SLoadInventory pkt;
	if (PacketHandlerUtils::ParsePacketHandler(pkt, Buffer, Header->GetSize() - Offset, Offset))
	{
		auto mode = Cast<ABJS_InGameMode>(GetWorld()->GetAuthGameMode());
		if (mode)
		{
			for (auto& item : pkt.itemequips())
			{
				mode->GetMyInventory()->AddEquipItem(
					item.unipeid(),
					item.item_code(),
					item.item_type(),
					item.attack(),
					item.speed(),
					item.is_equip(),
					item.position(),
					0);
				mode->UpdateInventoryEquipUI(item.unipeid(), 1);
				
				// 일단 여기서 아이템 스텟을 상승 시킨다.
				int32 attack = item.attack();
				int32 speed = item.speed();
				if (item.is_equip() == 1)
				{
					auto state = mode->GetMyState();
					state->ItemState.AddAttack(attack);
					state->ItemState.AddSpeed(speed);
				}
			}

			for (auto& item : pkt.itemetcs())
			{
				mode->GetMyInventory()->AddEtcItem(item.item_code(), item.item_type(), item.item_count(), item.position());
				mode->UpdateInventoryEtcUI(item.item_code(), 1);
			}
			mode->GetMyInventory()->SetGold(pkt.gold());
			mode->UpdateInventoryUI();
		}
	}
}

void ABJS_SocketActor::SellItemsHandler(BYTE* Buffer, PacketHeader* Header, int32 Offset)
{
	protocol::CSellItems pkt;

	if (PacketHandlerUtils::ParsePacketHandler(pkt, Buffer, Header->GetSize() - Offset, Offset))
	{
		auto mode = Cast<ABJS_InGameMode>(GetWorld()->GetAuthGameMode());
		if (mode)
		{
			for (auto& item : pkt.itemequips())
			{
				mode->GetMyInventory()->UseEquipItem(item.unipeid());
				mode->UpdateInventoryEquipUI(item.unipeid(), 0);
			}

			for (auto& item : pkt.itemetcs())
			{
				mode->GetMyInventory()->UseEtcItem(item.item_code(), item.item_count());
				mode->UpdateInventoryEtcUI(item.item_code(), 0);
			}

			mode->GetMyInventory()->SetGold(pkt.gold());
			mode->UpdateInventoryUI();
		}
	}
}

void ABJS_SocketActor::ChatHandler(BYTE* Buffer, PacketHeader* Header, int32 Offset)
{
	protocol::SChat pkt;

	if (PacketHandlerUtils::ParsePacketHandler(pkt, Buffer, Header->GetSize() - Offset, Offset))
	{
		OnChatMessage.Execute(UTF8_TO_TCHAR(pkt.text().c_str()), pkt.type(), pkt.uuid());
	}
}

void ABJS_SocketActor::ClosePlayerHandler(BYTE* Buffer, PacketHeader* Header, int32 Offset)
{
	protocol::SClosePlayer pkt;

	if (PacketHandlerUtils::ParsePacketHandler(pkt, Buffer, Header->GetSize() - Offset, Offset))
	{
		int32 removeUUid = pkt.uuid();
		OnDeSpawnDelegate.Execute(false, removeUUid);
	}
}

void ABJS_SocketActor::FriendSystemHandler(BYTE* Buffer, PacketHeader* Header, int32 Offset)
{
	protocol::SFriendSystem pkt;

	if (PacketHandlerUtils::ParsePacketHandler(pkt, Buffer, Header->GetSize() - Offset, Offset))
	{
		int32 result = pkt.result();
		if (result >= 0)
		{
			auto mode = Cast<ABJS_InGameMode>(GetWorld()->GetAuthGameMode());
			for (auto& myFriend : pkt.friend_())
			{
				int32 friendCode = myFriend.playercode();
				bool access = myFriend.access();
				int32 add = myFriend.add();

				if (mode)
				{
					if (add)
					{
						auto MyFriendRef = mode->GetMyFriend();
						if (MyFriendRef)
						{
							TCHAR* nameArr = UTF8_TO_TCHAR(myFriend.playername().c_str());
							MyFriendRef->AddFriend(friendCode, access, nameArr);
							MyFriendRef->UpdateFriend(friendCode, access);
						}
					}
					else
					{
						mode->GetMyFriend()->UpdateFriend(friendCode, access);
					}
				}
			}
			if (mode)
			{
				mode->UpdateFriendUi();
			}
		}
		else if (result < 0)
		{
			FString msg{TEXT("친구추가에 실패했습니다.")};
			OnChatMessage.Execute(msg, 2, -1);
		}
	}
}

void ABJS_SocketActor::UpdateItemsHandler(BYTE* Buffer, PacketHeader* Header, int32 Offset)
{
	protocol::CUpdateItems pkt;

	if (PacketHandlerUtils::ParsePacketHandler(pkt, Buffer, Header->GetSize() - Offset, Offset))
	{
		auto mode = Cast<ABJS_InGameMode>(GetWorld()->GetAuthGameMode());
		if (mode)
		{
			for (auto& item : pkt.itemequips())
			{
				mode->GetMyInventory()->ItemEquipped(item.unipeid(), item.is_equip(), item.position());
				mode->UpdateInventoryEquipUI(item.unipeid(), 2);

				// 일단 여기서 아이템 스텟을 상승 시킨다.
				int32 attack = item.attack();
				int32 speed = item.speed();
				if (!item.is_equip())
				{
					attack *= -1;
					speed *= -1;
				}
				auto state = mode->GetMyState();
				state->ItemState.AddAttack(attack);
				state->ItemState.AddSpeed(speed);
			}

			mode->UpdateInventoryUI();
		}
	}
}

void ABJS_SocketActor::LoadMailHandler(BYTE* Buffer, PacketHeader* Header, int32 Offset)
{
	protocol::CLoadMail pkt;
	if (PacketHandlerUtils::ParsePacketHandler(pkt, Buffer, Header->GetSize() - Offset, Offset))
	{
		auto mode = Cast<ABJS_InGameMode>(GetWorld()->GetAuthGameMode());
		if (mode)
		{
			auto MailBox = mode->GetMyMail();
			MailBox->AllRemoveMail();

			for (auto& mail : pkt.mails())
			{
				int32 code = mail.code();
				FString title = UTF8_TO_TCHAR(mail.title().c_str());
				FString message = UTF8_TO_TCHAR(mail.message().c_str());
				Mail NewMail{mail.code(),  mail.read(), mail.gold(), mail.socket1(), mail.socket1type(), mail.socket2(), mail.socket2type(), title, message};
				MailBox->AddMail(code, NewMail);
			}

			for (auto& mailItem : pkt.equipitems())
			{
				int32 mailCode = mailItem.mailcode();
				int32 socketPos = mailItem.socket();
				auto& item = mailItem.item();

				EquipItem newItem{item.unipeid(), item.item_code(), item.item_type(), item.attack(), item.speed(), item.is_equip(), item.position(), 0};
				MailBox->AddMailEquipItem(mailCode, socketPos, newItem);
			}

			for (auto& mailItem : pkt.etcitems())
			{
				int32 mailCode = mailItem.mailcode();
				int32 socketPos = mailItem.socket();
				auto& item = mailItem.item();

				EtcItem newItem{item.item_code(), item.item_type(), item.item_count(), item.position()};
				MailBox->AddMailEtcItem(mailCode, socketPos, newItem);
			}

			mode->UpdateMailUi();
		}
	}
}

void ABJS_SocketActor::UpdateMailHandler(BYTE* Buffer, PacketHeader* Header, int32 Offset)
{
	protocol::CUpdateMail pkt;
	if (PacketHandlerUtils::ParsePacketHandler(pkt, Buffer, Header->GetSize() - Offset, Offset))
	{
		auto mode = Cast<ABJS_InGameMode>(GetWorld()->GetAuthGameMode());
		if (mode)
		{
			auto MailBox = mode->GetMyMail();
			auto UpdateMail = pkt.mail();
			if (pkt.type() == 2)
			{
				auto mail = MailBox->GetMailList().Find(UpdateMail.code());
				if (mail)
				{
					mail->Socket1 = UpdateMail.socket1();
					mail->Socket2 = UpdateMail.socket2();
					mail->Gold = UpdateMail.gold();
				}
			}
			else if (pkt.type() == 3)
			{
				MailBox->RemoveMail(UpdateMail.code());
			}
			mode->UpdateMailUi();
		}
	}
}

void ABJS_SocketActor::UpdateSendMailHandler(BYTE* Buffer, PacketHeader* Header, int32 Offset)
{
	protocol::SSendMail pkt;
	
	if (PacketHandlerUtils::ParsePacketHandler(pkt, Buffer, Header->GetSize() - Offset, Offset))
	{
		auto mode = Cast<ABJS_InGameMode>(GetWorld()->GetAuthGameMode());
		if (mode)
		{
			if (pkt.result() > 0)
			{
				auto Inventory = mode->GetMyInventory();
				
				for (auto& item : pkt.itemequips())
				{
					mode->GetMyInventory()->UseEquipItem(item.unipeid());
					mode->UpdateInventoryEquipUI(item.unipeid(), 0);
				}
				
				Inventory->SetGold(pkt.gold());
				mode->UpdateInventoryUI();
			}
			else
			{
				// 에러 메시지 띄우기
				FString msg{TEXT("메일 보내기에 실패했습니다(해당되는 플레이어가 없습니다).")};
				OnChatMessage.Execute(msg, 2, -1);
			}
		}
		mode->UpdateMailUi();
	}
}
