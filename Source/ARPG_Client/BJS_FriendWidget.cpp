// Fill out your copyright notice in the Description page of Project Settings.


#include "BJS_FriendWidget.h"

#include "BJS_FriendSlotWidget.h"
#include "BJS_GameInstance.h"
#include "BJS_InGameMode.h"
#include "BJS_PromptWidget3.h"
#include "FriendSystem.h"
#include "GameClient.pb.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"

UBJS_FriendWidget::UBJS_FriendWidget()
{
	static ConstructorHelpers::FClassFinder<UUserWidget> UI_FRIENDSLOT_HUD(
		TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/MyGame/UMG/BJS_SWBP_FriendSlot.BJS_SWBP_FriendSlot_C'"));
	check(UI_FRIENDSLOT_HUD.Class);
	FriendSlotClass = UI_FRIENDSLOT_HUD.Class;
}

void UBJS_FriendWidget::BJS_InitWidget()
{
	if (btn_close)
		btn_close->OnClicked.AddDynamic(this, &UBJS_FriendWidget::OnClose);
	if (btn_friend)
		btn_friend->OnClicked.AddDynamic(this, &UBJS_FriendWidget::OnAddFriend);
	for (int32 i = 0; i < MaxFriendSize; i++)
	{
		auto slot = CreateWidget<UBJS_FriendSlotWidget>(GetWorld(), FriendSlotClass);
		sb_friendList->AddChild(slot);
	}
}

void UBJS_FriendWidget::NativeConstruct()
{
	Super::NativeConstruct();
	LoadFriendList();
}

void UBJS_FriendWidget::LoadFriendList()
{
	auto mode = Cast<ABJS_InGameMode>(GetWorld()->GetAuthGameMode());
	if (mode && sb_friendList->GetChildrenCount() == MaxFriendSize)
	{
		int32 i = 0;
		for (auto& entry : mode->GetMyFriend()->GetFriendList())
		{
			auto& Friend = entry.Value;
			auto slot = Cast<UBJS_FriendSlotWidget>(sb_friendList->GetChildAt(i));
			slot->SetText(Friend.Name);
			slot->SetState(Friend.Access);
			i++;
		}
		for (; i < 10; i++)
		{
			auto slot = Cast<UBJS_FriendSlotWidget>(sb_friendList->GetChildAt(i));
			slot->SetText(FString{});
		}
	}
}

void UBJS_FriendWidget::OnClose()
{
	SetVisibility(ESlateVisibility::Hidden);
	Cast<ABJS_GameModeBase>(GetWorld()->GetAuthGameMode())->SetShowMouseCousor(false);
}

void UBJS_FriendWidget::OnAddFriend()
{
	auto instance = Cast<UBJS_GameInstance>(GetGameInstance());
	if (instance)
	{
		auto Widget = CreateWidget<UBJS_PromptWidget3>(GetWorld(), instance->GetPrompt3());
		Widget->AddToViewport();
		Widget->SetText(Friend_Name_Str);
		Widget->OnSelectPromptEnd.BindUObject(this, &UBJS_FriendWidget::AddFriend);
	}
}

void UBJS_FriendWidget::AddFriend(FString Name)
{
	protocol::CFriend pkt;
	auto socketActor = Cast<ABJS_GameModeBase>(GetWorld()->GetAuthGameMode())->GetSocketActor();
	if (socketActor)
	{
		pkt.set_type(0);
		protocol::Friend* newFriend = new protocol::Friend();
		newFriend->set_add(true);
		newFriend->set_playername(TCHAR_TO_UTF8(*Name));
		
		pkt.set_allocated_friend_(newFriend);
		socketActor->SendMessage(pkt, protocol::MessageCode::C_FRIEND);
	}
}
