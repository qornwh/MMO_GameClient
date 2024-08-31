// Fill out your copyright notice in the Description page of Project Settings.


#include "BJS_MainWidget.h"

#include "BJS_CharaterState.h"
#include "BJS_GameInstance.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

UBJS_MainWidget::UBJS_MainWidget()
{
}

void UBJS_MainWidget::OnSelectEvent()
{
	OnSelect.Execute();
}

void UBJS_MainWidget::OnCStoreEvent()
{
	OnCStore.Execute();
}

void UBJS_MainWidget::OnWStoreEvent()
{
	OnWStore.Execute();
}

void UBJS_MainWidget::OnWStartEvent()
{
	OnStart.Execute();
}

void UBJS_MainWidget::SetLevel(int32 lv)
{
	tb_lv->SetText(FText::FromString(FString::FromInt(lv)));
}

void UBJS_MainWidget::SetName(FString name)
{
	tb_name->SetText(FText::FromString(name));
}

void UBJS_MainWidget::BJS_UpdateWidget()
{
	Super::BJS_UpdateWidget();
	
	if (auto myInstance = Cast<UBJS_GameInstance>(GetGameInstance()))
	{
		if (myInstance->GetMyState())
		{
			auto myState = myInstance->GetMyState();
			SetLevel(myState->GetLv());
			SetName(myState->GetName());
		}
	}
}

void UBJS_MainWidget::NativeConstruct()
{
	Super::NativeConstruct();
	btn_Select->OnClicked.AddDynamic(this, &UBJS_MainWidget::OnSelectEvent);
	btn_C_Store->OnClicked.AddDynamic(this, &UBJS_MainWidget::OnCStoreEvent);
	btn_W_Store->OnClicked.AddDynamic(this, &UBJS_MainWidget::OnWStoreEvent);
	btn_start->OnClicked.AddDynamic(this, &UBJS_MainWidget::OnWStartEvent);
}

