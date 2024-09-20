#pragma once

#include "CoreMinimal.h"
#include "InventoryItem.h"

struct Mail
{
	Mail(int32 code, int32 read, int32 gold, int32 socket1, int32 socket1Type, int32 socket2, int32 socket2Type, FString title, FString message);
	Mail(const Mail& other);
    
	Mail& operator=(const Mail& other);
	
	void SetEmptyItem();
	int32 Code;
	int32 Read;
	int32 Gold;
	int32 Socket1;
	int32 Socket1Type;
	int32 Socket2;
	int32 Socket2Type;
	FString Title;
	FString Message;
};

class ARPG_CLIENT_API MailBox : public TSharedFromThis<MailBox>
{
public:
	MailBox();
	~MailBox();

	TMap<int32, Mail>& GetMailList();
	void AddMail(int32 Code, Mail& Mail);
	void UpdateMail(int32 Code, Mail& Mail);
	void RemoveMail(int32 Code);
	void AllRemoveMail();

	TMap<TTuple<int32, int32>, EquipItem>& GetMailEquipList();
	TMap<TTuple<int32, int32>, EtcItem>& GetMailEtcList();
	void AddMailEquipItem(int32 MailCode, int32 SocketNum, EquipItem& Item);
	void AddMailEtcItem(int32 MailCode, int32 SocketNum, EtcItem& Item);
	void RemoveMailEtcItem(int32 MailCode, int32 SocketNum);
	void RemoveMailEquipItem(int32 MailCode, int32 SocketNum);

private:
	TMap<int32, Mail> MailList;
	TMap<TTuple<int32, int32>, EquipItem> MailEquipList;
	TMap<TTuple<int32, int32>, EtcItem> MailEtcList;
};
