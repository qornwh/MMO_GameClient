// Fill out your copyright notice in the Description page of Project Settings.


#include "BJS_LoginWidget.h"

#include "BJS_GameInstance.h"
#include "BJS_GameModeBase.h"
#include "BJS_LoginMode.h"
#include "BJS_PromptWidget.h"
#include "BJS_SocketActor.h"
#include "GameClient.pb.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"

void UBJS_LoginWidget::NativeConstruct()
{
	Super::NativeConstruct();

	btn_login->OnClicked.AddDynamic(this, &UBJS_LoginWidget::OnLoginEvent);
	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		PC->bShowMouseCursor = true;
		FInputModeGameAndUI InputMode;
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		PC->SetInputMode(InputMode);
	}
}

UBJS_LoginWidget::UBJS_LoginWidget(): tb_id(nullptr), tb_pw(nullptr), btn_login(nullptr), CurrentWidget(nullptr)
{
}

void UBJS_LoginWidget::BJS_UpdateWidget()
{
	Super::BJS_UpdateWidget();
}

void UBJS_LoginWidget::OnLoginEvent()
{
	ID = tb_id->GetText().ToString();
	PW = tb_pw->GetText().ToString();

	protocol::Login pkt;
	std::string idStr = TCHAR_TO_UTF8(*ID);
	std::string pwdStr = TCHAR_TO_UTF8(*PW);

	if (!ID.IsEmpty())
	{
		pkt.set_id(idStr);
		pkt.set_pwd(pwdStr);
		auto socketActor = Cast<ABJS_GameModeBase>(GetWorld()->GetAuthGameMode())->GetSocketActor();
		if (socketActor)
		{
			socketActor->SendMessage(pkt, protocol::MessageCode::LOGIN);
			socketActor->OnLoginAccess.BindUObject(this, &UBJS_LoginWidget::LoginCheck);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("SocketActor NotCreate!!!"));
		}
	}
}

void UBJS_LoginWidget::LoginCheck(int32 result)
{
	if (CurrentWidget)
	{
		CurrentWidget->RemoveFromParent();
		CurrentWidget = nullptr;
	}

	auto instance = Cast<UBJS_GameInstance>(GetGameInstance());
	if (!instance) return;

	if (result == 0 || result == 2)
	{
		// 로그인 실패, 계정 생성
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), instance->GetPrompt());
		if (CurrentWidget)
		{
			CurrentWidget->AddToViewport();
			auto widget = Cast<UBJS_PromptWidget>(CurrentWidget);
			if (widget)
			{
				FString str = FString(TEXT("로그인 실패 !!!"));
				if (result == 2)
					str = FString(TEXT("계정 생성 !!!"));
				widget->SetText(str);
			}
		}
	}
	else if (result == 1)
	{
		// 로그인 성공
		OnLoginSuccess.Execute();
	}
}
