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
		auto slot = CreateWidget<UBJS_FriendSlotWidget>(GetWorld(), FriendSlotClass);
		FriendSlotList.Add(slot);
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
	if (mode)
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
