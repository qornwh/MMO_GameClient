// Fill out your copyright notice in the Description page of Project Settings.


#include "BJS_FriendWidget.h"

#include "BJS_FriendSlotWidget.h"
#include "BJS_InGameMode.h"
#include "FriendSystem.h"
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
	for (int32 i = 0; i < MaxFriendSize; i++)
	{
		EmptyFriendSlot.Add(CreateWidget<UBJS_FriendSlotWidget>(GetWorld(), FriendSlotClass));
	}
	LoadFriendList();
}

void UBJS_FriendWidget::LoadFriendList()
{
	auto mode = Cast<ABJS_InGameMode>(GetWorld()->GetAuthGameMode());
	if (mode)
	{
		for (auto& person : mode->GetMyFriend()->GetFriendList())
		{
			AddFriend(person.Value);
		}
	}
}

void UBJS_FriendWidget::RemoveFriend(Person& person)
{
	int32 friendCode = person.PlayerCode;
	if (FriendSlot.Contains(friendCode))
	{
		auto slot = FriendSlot[friendCode];
		EmptyFriendSlot.Add(slot);
		FriendSlot.Remove(friendCode);
		sb_friendList->RemoveChild(Cast<UWidget>(slot));
		slot->Reset();
	}
}

void UBJS_FriendWidget::AddFriend(Person& person)
{
	if (!EmptyFriendSlot.IsEmpty())
	{
		auto slot = EmptyFriendSlot.Last();
		slot->SetText(person.Name);
		slot->SetState(person.Access);
		FriendSlot.Add(person.PlayerCode, slot);
		EmptyFriendSlot.Pop();
		sb_friendList->AddChild(slot);
	}
}

void UBJS_FriendWidget::UpdateFriend(Person& person)
{
	int32 friendCode = person.PlayerCode;
	if (FriendSlot.Contains(friendCode))
	{
		auto slot = FriendSlot[friendCode];
		slot->SetState(person.Access);
	}
}

void UBJS_FriendWidget::OnClose()
{
	SetVisibility(ESlateVisibility::Hidden);
	Cast<ABJS_GameModeBase>(GetWorld()->GetAuthGameMode())->SetShowMouseCousor(false);
}
