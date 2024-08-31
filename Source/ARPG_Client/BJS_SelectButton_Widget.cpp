// Fill out your copyright notice in the Description page of Project Settings.


#include "BJS_SelectButton_Widget.h"

#include "BJS_CreateChareter_Prompt_Widget.h"
#include "BJS_LoginMode.h"
#include "BJS_PromptWidget.h"
#include "BJS_SocketActor.h"
#include "GameClient.pb.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

UBJS_SelectButton_Widget::UBJS_SelectButton_Widget()
{
	static ConstructorHelpers::FClassFinder<UUserWidget> UI_PROMPT_HUD(
		TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/MyGame/UMG/BJS_WBP_Create.BJS_WBP_Create_C'"));
	if (UI_PROMPT_HUD.Succeeded())
	{
		PromptClass = UI_PROMPT_HUD.Class;
	}
}

void UBJS_SelectButton_Widget::BJS_UpdateWidget()
{
	Super::BJS_UpdateWidget();
}

void UBJS_SelectButton_Widget::SetType(int32 CWType)
{
	Type = CWType;
}

void UBJS_SelectButton_Widget::SetSelectBtn(bool Select, bool Own)
{
	IsSelect = Select;
	IsOwn = Own;
	SetOpacity();

	if (IsSelect)
		tb->SetText(FText::FromString(Select_Select_Str));
}

void UBJS_SelectButton_Widget::SetBuyBtn(bool Own)
{
	IsOwn = Own;
	SetOpacity();

	if (IsOwn)
		tb->SetText(FText::FromString(Select_Own_Str));
	else
		tb->SetText(FText::FromString(Select_No_Str));

	if (IsSelect)
		tb->SetText(FText::FromString(Select_Select_Str));
}

void UBJS_SelectButton_Widget::SetImageBtn(UTexture2D* Image)
{
	if (Image)
	{
		FSlateBrush Brush;
		Brush.SetResourceObject(Image);
		Brush.ImageSize = FVector2D(Image->GetSizeX(), Image->GetSizeY());

		FButtonStyle Style;

		Style.Normal.SetResourceObject(Image);
		Style.Normal = Brush;
		Style.Hovered = Brush;
		Style.Pressed = Brush;

		btn->SetStyle(Style);
	}
}

bool UBJS_SelectButton_Widget::GetSelect()
{
	return IsSelect;
}

bool UBJS_SelectButton_Widget::GetOwn()
{
	return IsOwn;
}

void UBJS_SelectButton_Widget::CreateCharater(FString Name)
{
	protocol::CBuyCharater pkt;
	auto socketActor = Cast<ABJS_GameModeBase>(GetWorld()->GetAuthGameMode())->GetSocketActor();
	if (socketActor)
	{
		pkt.set_usecash(pay);
		pkt.set_charatertype(Type);
		std::string nameStr = TCHAR_TO_UTF8(*Name);
		pkt.set_name(nameStr);
		socketActor->SendMessage(pkt, protocol::MessageCode::C_BUYCHARATER);
		TCHAR* tcharArr = UTF8_TO_TCHAR(pkt.name().c_str());
	}
}

UButton* UBJS_SelectButton_Widget::GetBtn()
{
	return btn;
}

void UBJS_SelectButton_Widget::OnSelectBtn()
{
	if (IsOwn)
	{
		if (!IsSelect)
		{
			protocol::CUpdateAccount pkt;
			auto socketActor = Cast<ABJS_GameModeBase>(GetWorld()->GetAuthGameMode())->GetSocketActor();
			if (socketActor)
			{
				if (Type <= 3)
					pkt.set_charatertype(Type);
				else
					pkt.set_charatertype(0);

				if (Type > 3)
					pkt.set_weapontype(Type - 3);
				else
					pkt.set_weapontype(0);

				pkt.set_usecash(0);
				socketActor->SendMessage(pkt, protocol::MessageCode::C_UPDATEACCOUNT);
			}
		}
	}
}

void UBJS_SelectButton_Widget::OnBuyBtn()
{
	if (!IsOwn)
	{
		if (Type <= 3)
		{
			if (PromptClass)
			{
				auto CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), PromptClass);
				auto widget = Cast<UBJS_CreateChareter_Prompt_Widget>(CurrentWidget);

				if (widget)
				{
					widget->AddToViewport();
					widget->SetText(Create_Name_Str);
					widget->OnSelectPromptEnd.BindUObject(this, &UBJS_SelectButton_Widget::CreateCharater);
				}
			}
		}
		else
		{
			protocol::CBuyWeapon pkt;
			auto socketActor = Cast<ABJS_GameModeBase>(GetWorld()->GetAuthGameMode())->GetSocketActor();
			if (socketActor)
			{
				pkt.set_usecash(pay);
				pkt.set_weapontype(Type - 3);
				socketActor->SendMessage(pkt, protocol::MessageCode::C_BUYWEAPON);
			}
		}
	}
}

void UBJS_SelectButton_Widget::SetOpacity()
{
	if (IsOwn)
	{
		btn->SetRenderOpacity(1.f);
		tb->SetText(FText::FromString(Select_Own_Str));
	}
	else
	{
		btn->SetRenderOpacity(Opactiy);
		tb->SetText(FText::FromString(Select_No_Str));
	}
}
