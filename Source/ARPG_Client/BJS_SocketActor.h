// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BJS_Buffer.h"
#include "PacketHeader.h"
#include "GameFramework/Actor.h"
#include "google/protobuf/message.h"
#include "BJS_SocketActor.generated.h"

DECLARE_DELEGATE_OneParam(FOnLoginResultDelegate, int32)
DECLARE_DELEGATE(FOnLoadDelegate);
DECLARE_DELEGATE_TwoParams(FOnSpawnDelegate, bool, int32);
DECLARE_DELEGATE_TwoParams(FOnDemageDelegate, int32, int32);
DECLARE_DELEGATE_OneParam(FOnMoveStartPoint, AActor*);
DECLARE_DELEGATE_ThreeParams(FOnChatMessage, FString, int32, int32);

UCLASS()
class ARPG_CLIENT_API ABJS_SocketActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABJS_SocketActor();

	// 로그인용
	FOnLoginResultDelegate OnLoginAccess;
	// 처음 로드시 다른캐릭터, 몬스터 스폰용
	FOnLoadDelegate OnLoadDelegate;
	// 몬스터 플레이어 스폰용
	FOnSpawnDelegate OnSpawnDelegate;
	// 스폰된 몬스터 플레이어 제거용
	FOnSpawnDelegate OnDeSpawnDelegate;
	// 캐릭터 포인터로 강제 이동
	FOnMoveStartPoint OnMoveStartPoint;
	// 캐릭터 포인터로 강제 이동
	FOnChatMessage OnChatMessage;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	void HandlePacket(BYTE* Buffer, PacketHeader* Header);
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void SendMessage(google::protobuf::Message& Pkt, uint16 Id);
	ESocketConnectionState GetConnectionState();
	void Close();

private:
	void ReConnectSocket();
	void LoginAccessHandler(BYTE* Buffer, PacketHeader* Header, int32 Offset);
	void CreateAccountHandler(BYTE* Buffer, PacketHeader* Header, int32 Offset);
	void CurrentInfoHandler(BYTE* Buffer, PacketHeader* Header, int32 Offset);
	void BuyResultHandler(BYTE* Buffer, PacketHeader* Header, int32 Offset);
	void PlayerDataHandler(BYTE* Buffer, PacketHeader* Header, int32 Offset);
	void LoadHandler(BYTE* Buffer, PacketHeader* Header, int32 Offset);
	void InsertPlayerHandler(BYTE* Buffer, PacketHeader* Header, int32 Offset);
	void CharaterUpdateHandler(BYTE* Buffer, PacketHeader* Header, int32 Offset);
	void MoveHandler(BYTE* Buffer, PacketHeader* Header, int32 Offset);
	void AttackHandler(BYTE* Buffer, PacketHeader* Header, int32 Offset);
	void CharaterDemageHandler(BYTE* Buffer, PacketHeader* Header, int32 Offset);
	void PlayerAimHandler(BYTE* Buffer, PacketHeader* Header, int32 Offset);
	void PlayerJumpHandler(BYTE* Buffer, PacketHeader* Header, int32 Offset);
	void ExpLvHandler(BYTE* Buffer, PacketHeader* Header, int32 Offset);
	void DropMessageHandler(BYTE* Buffer, PacketHeader* Header, int32 Offset);
	void LoadInventoryHandler(BYTE* Buffer, PacketHeader* Header, int32 Offset);
	void SellItemsHandler(BYTE* Buffer, PacketHeader* Header, int32 Offset);
	void ChatHandler(BYTE* Buffer, PacketHeader* Header, int32 Offset);

private:
	FSocket* MySocket = nullptr;
	BJS_BufferPtr ReadBuffer;
	BJS_BufferPtr WriteBuffer;
};
