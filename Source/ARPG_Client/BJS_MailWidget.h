// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BJS_UserWidgetBase.h"
#include "BJS_MailWidget.generated.h"

UCLASS()
class ARPG_CLIENT_API UBJS_MailWidget : public UBJS_UserWidgetBase
{
	GENERATED_BODY()

	enum MailMode : int32
	{
		RECIVE = 0,
		SEND = 1,
	};

public:
	UBJS_MailWidget();
	~UBJS_MailWidget();

	virtual void BJS_InitWidget() override;
	virtual void BJS_UpdateWidget() override;

	UFUNCTION()
	void OnHiddenMailBox();
	UFUNCTION()
	void ViewMailRecive();
	UFUNCTION()
	void ViewMailSend();

	UFUNCTION()
	void ReceiveItem();
	UFUNCTION()
	void ReceiveItemAll();
	UFUNCTION()
	void RemoveMail();
	UFUNCTION()
	void RemoveMailAll();
	UFUNCTION()
	void RefreshMail();
	UFUNCTION()
	void SendMail();

	void SetSlot();
	void ResetMails();
	void SetMailInfo(int32 MailCode);
	void SetMailSocketInfo(int32 SocketNum, int32 SocketState, int32 SocketType);
	void SetGold(int32 Gold);

protected:
	virtual void NativeConstruct() override;
	void SetViewMailInfo(bool Flag);
	void SetBtnReciveImage(TObjectPtr<class UTexture2D> Image);
	void SetBtnSendImage(TObjectPtr<class UTexture2D> Image);

private:
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UBJS_HeaderWidget* header;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UButton* btn_recive_tab;
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UHorizontalBox* hb_recive;
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UButton* btn_send_tab;
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UHorizontalBox* hb_send;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UScrollBox* sb_mail;
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UButton* btn_all_recive;
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UButton* btn_all_remove;
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UButton* btn_refresh;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UCanvasPanel* cp_info;
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UTextBlock* tb_mail_title;
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UMultiLineEditableTextBox* mtb_mail_message;
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UBJS_ItemSlotWidget* socket1;
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UBJS_ItemSlotWidget* socket2;
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UTextBlock* tb_recive_gold;
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UButton* btn_mail_recive;
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UButton* btn_mail_remove;

	// send
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UEditableText* etb_send_gold;
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UButton* btn_send_mail;

	TSubclassOf<class UBJS_MailSlot> MailSlotClass;
	TArray<TObjectPtr<class UBJS_MailSlot>> MailSlotList;
	TMap<int32, TObjectPtr<class UBJS_MailSlot>> MailSlots;

	TArray<TObjectPtr<class UTexture2D>> EnableMailButtonImageList;

	MailMode ModeState = MailMode::RECIVE;
	const int32 MaxMailBoxSize = 10;
	int32 CurrentMailCode = -1;
};
