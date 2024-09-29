// Fill out your copyright notice in the Description page of Project Settings.


#include "BJS_MailWidget.h"

#include "BJS_GameInstance.h"
#include "BJS_HeaderWidget.h"
#include "BJS_MailSlot.h"
#include "BJS_InGameMode.h"
#include "BJS_ItemSlotWidget.h"
#include "BJS_SubInventoryWidget.h"
#include "GameClient.pb.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/EditableText.h"
#include "Components/HorizontalBox.h"
#include "Components/MultiLineEditableText.h"
#include "Components/MultiLineEditableTextBox.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"

UBJS_MailWidget::UBJS_MailWidget()
{
	static ConstructorHelpers::FClassFinder<UUserWidget> MAIL_SLOT_HUD(
		TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/MyGame/UMG/BJS_SWBP_MailISlot.BJS_SWBP_MailISlot_C'"));
	check(MAIL_SLOT_HUD.Class);

	static ConstructorHelpers::FObjectFinder<UTexture2D> BTN_MAIL_OFF_IMAGE(
		TEXT("/Script/Engine.Texture2D'/Game/Cyberpunk_RPG_GUI_Resources/SlicedElements/04Hero_Attribute/Btn_MainMenu_n.Btn_MainMenu_n'"));
	check(BTN_MAIL_OFF_IMAGE.Object);

	static ConstructorHelpers::FObjectFinder<UTexture2D> BTN_MAIL_ON_IMAGE(
		TEXT("/Script/Engine.Texture2D'/Game/Cyberpunk_RPG_GUI_Resources/SlicedElements/04Hero_Attribute/Btn_MainMenu_s.Btn_MainMenu_s'"));
	check(BTN_MAIL_ON_IMAGE.Object);

	MailSlotClass = MAIL_SLOT_HUD.Class;
	EnableMailButtonImageList.Add(BTN_MAIL_OFF_IMAGE.Object);
	EnableMailButtonImageList.Add(BTN_MAIL_ON_IMAGE.Object);
}

UBJS_MailWidget::~UBJS_MailWidget()
{
}

void UBJS_MailWidget::BJS_InitWidget()
{
	Super::BJS_InitWidget();
	sub_inventory->BJS_InitWidget();

	FMargin padding{30, 30, 30, 0};
	for (int32 i = 0; i < MaxMailBoxSize; i++)
	{
		auto slot = CreateWidget<UBJS_MailSlot>(GetWorld(), MailSlotClass);
		slot->SetSlot(false);
		slot->SetPadding(padding);
		slot->SetParentWidget(this);
		MailSlotList.Add(slot);
		sb_mail->AddChild(slot);
	}

	SetSlot();
	SetViewMailInfo(false);
	ViewMailRecive();
}

void UBJS_MailWidget::BJS_UpdateWidget()
{
	Super::BJS_UpdateWidget();

	auto instance = Cast<UBJS_GameInstance>(GetGameInstance());
	if (instance)
	{
		header->SetTextCash(instance->GetCash());
		header->SetTextGold(instance->GetMyInventory()->GetGold());
	}
}

void UBJS_MailWidget::SetVisibility(ESlateVisibility InVisibility)
{
	Super::SetVisibility(InVisibility);
	if (InVisibility == ESlateVisibility::Visible)
	{
		SetSlot();
	}
}

void UBJS_MailWidget::SetSlot()
{
	auto mode = Cast<ABJS_InGameMode>(GetWorld()->GetAuthGameMode());

	if (mode)
	{
		auto MailBox = mode->GetMyMail();
		ResetMails();

		int32 i = 0;
		for (auto& entry : MailBox->GetMailList())
		{
			auto& mail = entry.Value;
			auto slot = MailSlotList[i];
			slot->SetMail(mail);
			slot->SetSlot(true);
			i++;
		}

		SetMailInfo(CurrentMailCode);
		etb_title->SetText(FText());
		metb_message->SetText(FText());
		etb_send_gold->SetText(FText::FromString(L"0"));
		sub_inventory->ReLoadSlot();
		send_socket1->SetSlots(false);
		send_socket2->SetSlots(false);
	}
}

void UBJS_MailWidget::ResetMails()
{
	for (auto slot : MailSlotList)
	{
		slot->SetSlot(false);
	}
}

void UBJS_MailWidget::SetViewMailInfo(bool Flag)
{
	if (Flag)
		cp_info->SetVisibility(ESlateVisibility::Visible);
	else
		cp_info->SetVisibility(ESlateVisibility::Hidden);
}

void UBJS_MailWidget::SetBtnReciveImage(TObjectPtr<UTexture2D> Image)
{
	if (Image)
	{
		FSlateBrush Brush;
		Brush.SetResourceObject(Image);
		Brush.ImageSize = FVector2D(Image->GetSizeX(), Image->GetSizeY());
		FButtonStyle Style = btn_recive_tab->GetStyle();
		Style.Normal.SetResourceObject(Image);
		btn_recive_tab->SetStyle(Style);
	}
}

void UBJS_MailWidget::SetBtnSendImage(TObjectPtr<UTexture2D> Image)
{
	if (Image)
	{
		FSlateBrush Brush;
		Brush.SetResourceObject(Image);
		Brush.ImageSize = FVector2D(Image->GetSizeX(), Image->GetSizeY());
		FButtonStyle Style = btn_send_tab->GetStyle();
		Style.Normal.SetResourceObject(Image);
		btn_send_tab->SetStyle(Style);
	}
}

void UBJS_MailWidget::OnHiddenMailBox()
{
	auto mode = Cast<ABJS_InGameMode>(GetWorld()->GetAuthGameMode());
	if (mode)
	{
		mode->ChangeMailBoxUI();
	}
}

void UBJS_MailWidget::ViewMailRecive()
{
	ModeState = MailMode::RECIVE;
	hb_recive->SetVisibility(ESlateVisibility::Visible);
	hb_send->SetVisibility(ESlateVisibility::Hidden);
	SetBtnReciveImage(EnableMailButtonImageList[1]);
	SetBtnSendImage(EnableMailButtonImageList[0]);
}

void UBJS_MailWidget::ViewMailSend()
{
	ModeState = MailMode::SEND;
	hb_recive->SetVisibility(ESlateVisibility::Hidden);
	hb_send->SetVisibility(ESlateVisibility::Visible);
	SetBtnReciveImage(EnableMailButtonImageList[0]);
	SetBtnSendImage(EnableMailButtonImageList[1]);
}

void UBJS_MailWidget::ReceiveItem()
{
	if (CurrentMailCode > 0)
	{
		auto mode = Cast<ABJS_InGameMode>(GetWorld()->GetAuthGameMode());
		if (mode)
		{
			mode->UpdateMail(CurrentMailCode, 2);
		}
	}
}

void UBJS_MailWidget::ReceiveItemAll()
{
	auto mode = Cast<ABJS_InGameMode>(GetWorld()->GetAuthGameMode());
	if (mode)
	{
		mode->AllUpdateMail(2);
	}
}

void UBJS_MailWidget::RemoveMail()
{
	if (CurrentMailCode > 0)
	{
		auto mode = Cast<ABJS_InGameMode>(GetWorld()->GetAuthGameMode());
		if (mode)
		{
			mode->UpdateMail(CurrentMailCode, 3);
		}
	}
}

void UBJS_MailWidget::RemoveMailAll()
{
	auto mode = Cast<ABJS_InGameMode>(GetWorld()->GetAuthGameMode());
	if (mode)
	{
		mode->AllUpdateMail(3);
	}
}

void UBJS_MailWidget::RefreshMail()
{
	auto mode = Cast<ABJS_InGameMode>(GetWorld()->GetAuthGameMode());
	if (mode)
	{
		mode->AllUpdateMail(1);
	}
}

void UBJS_MailWidget::SendMail()
{
	
	auto mode = Cast<ABJS_InGameMode>(GetWorld()->GetAuthGameMode());
	if (mode)
	{
		if (!etb_send_gold->GetText().IsNumeric())
		{
			UE_LOG(LogTemp, Log, TEXT("골드칸에 숫자만 입력해 주세요 !!!"));
			return;
		}
		
		int32 SendGold = FCString::Atoi(*etb_send_gold->GetText().ToString());
		if (mode->GetMyInventory()->GetGold() < SendGold)
		{
			UE_LOG(LogTemp, Log, TEXT("골드가 부족합니다 !!!"));
			return;
		}
		
		auto socket = mode->GetSocketActor();
		if (socket)
		{
			protocol::CSendMail pkt;
			protocol::Mail* mail = new protocol::Mail();
			mail->set_gold(SendGold);
			mail->set_message(TCHAR_TO_UTF8(*metb_message->GetText().ToString()));
			mail->set_title(TCHAR_TO_UTF8(*etb_title->GetText().ToString()));
			pkt.set_allocated_mails(mail);
			if (!send_socket1->GetEquip().IsEmpty())
			{
				auto& sendItem = send_socket1->GetEquip();
				protocol::MailEquipItem* MailItem = pkt.add_equipitems();
				mail->set_socket1(1);
				mail->set_socket1type(sendItem.EquipType);
				protocol::ItemEquip* Item = new protocol::ItemEquip();
				Item->set_item_code(sendItem.ItemCode);
				Item->set_item_type(sendItem.EquipType);
				Item->set_attack(sendItem.Attack);
				Item->set_speed(sendItem.Speed);
				Item->set_position(1);
				Item->set_unipeid(sendItem.UniqueId);
				MailItem->set_allocated_item(Item);
			}
			if (!send_socket2->GetEquip().IsEmpty())
			{
				auto& item = send_socket2->GetEquip();
				protocol::MailEquipItem* MailItem = pkt.add_equipitems();
				mail->set_socket2(1);
				mail->set_socket2type(item.EquipType);
				protocol::ItemEquip* Item = new protocol::ItemEquip();
				Item->set_item_code(item.ItemCode);
				Item->set_item_type(item.EquipType);
				Item->set_attack(item.Attack);
				Item->set_speed(item.Speed);
				Item->set_position(2);
				Item->set_unipeid(item.UniqueId);
				MailItem->set_allocated_item(Item);
			}

			mode->GetSocketActor()->SendMessage(pkt, protocol::MessageCode::C_SENDMAIL);
		}
	}
}

void UBJS_MailWidget::SetMailInfo(int32 MailCode)
{
	auto instance = Cast<UBJS_GameInstance>(GetGameInstance());
	if (instance)
	{
		auto MailBox = instance->GetMyMail();
		auto mail = MailBox->GetMailList().Find(MailCode);
		if (mail != nullptr)
		{
			CurrentMailCode = mail->Code;
			tb_mail_title->SetText(FText::FromString(mail->Title));
			mtb_mail_message->SetText(FText::FromString(mail->Message));
			SetGold(mail->Gold);
			SetMailSocketInfo(1, mail->Socket1, mail->Socket1Type);
			SetMailSocketInfo(2, mail->Socket2, mail->Socket2Type);
			SetViewMailInfo(true);
		}
		else
		{
			SetViewMailInfo(false);
		}
	}
}

void UBJS_MailWidget::SetMailSocketInfo(int32 SocketNum, int32 SocketState, int32 SocketType)
{
	auto instance = Cast<UBJS_GameInstance>(GetGameInstance());
	if (instance)
	{
		auto Socket = SocketNum == 1 ? socket1 : socket2;
		auto MailBox = instance->GetMyMail();
		auto EquipImgs = instance->GetItemEquipIconImgMap();
		auto EtcImgs = instance->GetItemEtcIconImgMap();

		if (SocketType == 1)
		{
			auto equip = MailBox->GetMailEquipList().Find({CurrentMailCode, SocketNum});
			if (equip)
			{
				Socket->SetEquip(*equip);
				Socket->SetImg(EquipImgs[equip->ItemCode]);
			}
		}
		else if (SocketType == 2)
		{
			auto etc = MailBox->GetMailEtcList().Find({CurrentMailCode, SocketNum});
			if (etc)
			{
				Socket->SetEtc(*etc);
				Socket->SetImg(EtcImgs[etc->ItemCode]);
			}
		}
		else
		{
			Socket->SetImg(nullptr);
		}

		if (SocketState == 0)
			Socket->SetRecive(false);
		else if (SocketState == 1)
			Socket->SetRecive(true);
	}
}

void UBJS_MailWidget::SetGold(int32 Gold)
{
	FString goldStr = FString::FromInt(Gold);
	tb_recive_gold->SetText(FText::FromString(goldStr));
}

void UBJS_MailWidget::SetSendMailEquipItem(int32 EquipUnipeId, int32 Position)
{
	auto instance = Cast<UBJS_GameInstance>(GetGameInstance());
	if (instance)
	{
		auto item = instance->GetMyInventory()->GetEquipItems().Find(EquipUnipeId);
		if (item)
		{
			auto EquipImgs = instance->GetItemEquipIconImgMap();
			UBJS_ItemSlotWidget* socket = nullptr;
			if (Position == 1)
				socket = send_socket1;
			else
				socket = send_socket2;
			
			socket->SetEquip(*item);
			socket->SetSocket(true);
			socket->SetImg(EquipImgs[item->ItemCode]);
		}
	}
}

void UBJS_MailWidget::NativeConstruct()
{
	Super::NativeConstruct();

	header->SetTextTitle("MailBox");
	header->OnBack.BindUObject(this, &UBJS_MailWidget::OnHiddenMailBox);
	btn_refresh->OnClicked.AddDynamic(this, &UBJS_MailWidget::RefreshMail);
	btn_mail_recive->OnClicked.AddDynamic(this, &UBJS_MailWidget::ReceiveItem);
	btn_all_recive->OnClicked.AddDynamic(this, &UBJS_MailWidget::ReceiveItemAll);
	btn_mail_remove->OnClicked.AddDynamic(this, &UBJS_MailWidget::RemoveMail);
	btn_all_remove->OnClicked.AddDynamic(this, &UBJS_MailWidget::RemoveMailAll);
	btn_recive_tab->OnClicked.AddDynamic(this, &UBJS_MailWidget::ViewMailRecive);
	btn_send_tab->OnClicked.AddDynamic(this, &UBJS_MailWidget::ViewMailSend);
	btn_send_mail->OnClicked.AddDynamic(this, &UBJS_MailWidget::SendMail);

	send_socket1->SetSocketType(ITEMSOCKETTYPE::SENDMAIL);
	send_socket1->SetSocketPosition(1);
	send_socket1->SetSocket(false);
	send_socket2->SetSocketType(ITEMSOCKETTYPE::SENDMAIL);
	send_socket2->SetSocketPosition(2);
	send_socket2->SetSocket(false);
}
