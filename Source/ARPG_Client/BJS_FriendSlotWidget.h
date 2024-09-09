// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BJS_SubWidget.h"
#include "BJS_FriendSlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_CLIENT_API UBJS_FriendSlotWidget : public UBJS_SubWidget
{
	GENERATED_BODY()

	enum state
	{
		whith = 0,
		green = 1
	};

public:
	void BJS_InitWidget() override;
	void SetText(FString text);
	void SetState(int32 color);
	void Reset();
	
private:
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UButton* btn_friend;
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UTextBlock* tb_friend;
};
