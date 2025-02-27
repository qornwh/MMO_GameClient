// Fill out your copyright notice in the Description page of Project Settings.


#include "BJS_GameModeBase.h"

#include "GameFramework/PlayerStart.h"
#include "BJS_SocketActor.h"
#include "BJS_UserWidgetBase.h"
#include "BJS_GameInstance.h"
#include "BJS_PromptWidget.h"
#include "BJS_PromptWidget2.h"
#include "EngineUtils.h"

ABJS_GameModeBase::ABJS_GameModeBase()
{
	static ConstructorHelpers::FClassFinder<ABJS_SocketActor> ACTOR_SOCKET(TEXT("/Script/CoreUObject.Class'/Script/ARPG_Client.BJS_SocketActor'"));
	check(ACTOR_SOCKET.Succeeded())
	SocketClass = ACTOR_SOCKET.Class;
	ShowMouseCursor = false;
}

void ABJS_GameModeBase::BeginPlay()
{
	Super::BeginPlay();
	
	for (TActorIterator<APlayerStart> It(GetWorld()); It; ++It)
	{
		PlayerStart = *It;
		if (PlayerStart)
			break;
	}
	check(PlayerStart);
}

void ABJS_GameModeBase::Destroyed()
{
	Super::Destroyed();
}

void ABJS_GameModeBase::LoadSubClass()
{
}

ABJS_SocketActor* ABJS_GameModeBase::GetSocketActor()
{
	return SocketActor;
}

UBJS_UserWidgetBase* ABJS_GameModeBase::GetCurrentWidget()
{
	return CurrentWidget;
}

void ABJS_GameModeBase::SetCurrentWidget(UBJS_UserWidgetBase* currentWidget)
{
	CurrentWidget = currentWidget;
}

UBJS_PromptWidget* ABJS_GameModeBase::OpenPromptWidget()
{
	auto instance = Cast<UBJS_GameInstance>(GetGameInstance());
	if (!instance)
		return nullptr;
	
	if (PromptWidget == nullptr)
	{
		PromptWidget = CreateWidget<UBJS_PromptWidget>(GetWorld(), instance->GetPrompt());
		check(PromptWidget);
	}
	PromptWidget->AddToViewport();
	return PromptWidget;
}

UBJS_PromptWidget2* ABJS_GameModeBase::OpenPromptWidget2()
{
	auto instance = Cast<UBJS_GameInstance>(GetGameInstance());
	if (!instance)
		return nullptr;
	
	if (PromptWidget2 == nullptr)
	{
		PromptWidget2 = CreateWidget<UBJS_PromptWidget2>(GetWorld(), instance->GetPrompt2());
		check(PromptWidget2);
	}
	PromptWidget2->AddToViewport();
	return PromptWidget2;
}

void ABJS_GameModeBase::MoveStartPoint(AActor* Actor)
{
	if (PlayerStart)
	{
		// 일단 그냥 강제 이동시킨다.
		Actor->SetActorLocation(PlayerStart->GetActorLocation());	
	}
}

APlayerController* ABJS_GameModeBase::GetPlayerController()
{
	if (GetWorld())
	{
		return GetWorld()->GetFirstPlayerController();
	}
	return nullptr;
}

void ABJS_GameModeBase::SetShowMouseCousor(bool Flag)
{
	auto control = GetPlayerController();
	if (!control)
		return;
	ShowMouseCursor = Flag;
	control->SetShowMouseCursor(ShowMouseCursor);
	if (Flag)
	{
		if (CurrentWidget)
		{
			control->SetInputMode(FInputModeGameAndUI());
		}
	}
	else
	{
		control->SetInputMode(FInputModeGameOnly());
	}
}
