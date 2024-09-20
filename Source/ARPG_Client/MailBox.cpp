#include "MailBox.h"

Mail::Mail(int32 code, int32 read, int32 gold, int32 socket1, int32 socket1Type, int32 socket2, int32 socket2Type, FString title, FString message)
	: Code(code), Read(read), Gold(gold), Socket1(socket1), Socket1Type(socket1Type), Socket2(socket2), Socket2Type(socket2Type), Title(title), Message(message)
{
}

Mail::Mail(const Mail& other) : Code(other.Code), Read(other.Read), Gold(other.Gold), Socket1(other.Socket1), Socket1Type(other.Socket1Type),
                                Socket2(other.Socket2), Socket2Type(other.Socket2Type), Title(other.Title), Message(other.Message)
{
}

Mail& Mail::operator=(const Mail& other)
{
	Code = other.Code;
	Read = other.Read;
	Title = other.Title;
	Message = other.Message;
	Gold = other.Gold;
	Socket1 = other.Socket1;
	Socket1Type = other.Socket1Type;
	Socket2 = other.Socket2;
	Socket2Type = other.Socket2Type;

	return *this;
}

void Mail::SetEmptyItem()
{
	Code = -1;
	Read = -1;
	Title = FString();
	Message = FString();
	Gold = 0;
	Socket1 = -1;
	Socket1Type = -1;
	Socket2 = -1;
	Socket2Type = -1;
}

MailBox::MailBox()
{
}

MailBox::~MailBox()
{
}

TMap<int32, Mail>& MailBox::GetMailList()
{
	return MailList;
}

void MailBox::AddMail(int32 Code, Mail& Mail)
{
	MailList.Add(Code, Mail);
}

void MailBox::UpdateMail(int32 Code, Mail& Mail)
{
	MailList[Code] = Mail;
}

void MailBox::RemoveMail(int32 Code)
{
	MailList.Remove(Code);
}

void MailBox::AllRemoveMail()
{
	MailList.Empty();
	MailEquipList.Empty();
	MailEtcList.Empty();
}

TMap<TTuple<int32, int32>, EquipItem>& MailBox::GetMailEquipList()
{
	return MailEquipList;
}

TMap<TTuple<int32, int32>, EtcItem>& MailBox::GetMailEtcList()
{
	return MailEtcList;
}

void MailBox::AddMailEquipItem(int32 MailCode, int32 SocketNum, EquipItem& Item)
{
	MailEquipList.Add({MailCode, SocketNum}, Item);
}

void MailBox::AddMailEtcItem(int32 MailCode, int32 SocketNum, EtcItem& Item)
{
	MailEtcList.Add({MailCode, SocketNum}, Item);
}

void MailBox::RemoveMailEtcItem(int32 MailCode, int32 SocketNum)
{
	MailEquipList.Remove({MailCode, SocketNum});
}

void MailBox::RemoveMailEquipItem(int32 MailCode, int32 SocketNum)
{
	MailEtcList.Remove({MailCode, SocketNum});
}
