// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BJS_SubWidget.h"
#include "BJS_FriendWidget.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_CLIENT_API UBJS_FriendWidget : public UBJS_SubWidget
{
	GENERATED_BODY()

public:
	UBJS_FriendWidget();
	virtual void BJS_InitWidget() override;
	virtual void NativeConstruct() override;
	void LoadFriendList();
	
	UFUNCTION()
	void OnClose();
	UFUNCTION()
	void OnAddFriend();
	UFUNCTION()
	void AddFriend(FString Name);

private:
	TSubclassOf<class UBJS_FriendSlotWidget> FriendSlotClass;
	TMap<int32, TObjectPtr<UBJS_FriendSlotWidget>> FriendSlot;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UScrollBox* sb_friendList;
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UButton* btn_close;
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UButton* btn_friend;

	FString Friend_Name_Str = TEXT("추가할 친구의 이름을 입력해주세요.");
	const int32 MaxFriendSize = 10;
};
