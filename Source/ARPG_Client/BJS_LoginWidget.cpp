#include "BJS_LoginWidget.h"

#include "BJS_GameModeBase.h"
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

UBJS_LoginWidget::UBJS_LoginWidget()
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
	auto mode = Cast<ABJS_GameModeBase>(GetWorld()->GetAuthGameMode());
	if (result == 1)
	{
		// 로그인 성공
		OnLoginSuccess.Execute();
	}
	else
	{
		auto Widget = mode->OpenPromptWidget();
		FString str;
		if (result == 0)
			str = FString(TEXT("로그인 실패 - 비밀번호가 틀렸습니다 !!!"));
		else if (result == 2)
			str = FString(TEXT("계정 생성 성공 !!!"));
		else if (result == 3)
			str = FString(TEXT("로그인 실패 - 이미 로그인된 계정 !!!"));
		else if (result == 4)
			str = FString(TEXT("로그인 실패 - 계정 생성에 실패 !!!"));
		Widget->SetText(str);
	}
}
