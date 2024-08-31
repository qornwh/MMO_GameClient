// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BJS_SubWidget.h"
#include "BJS_StateWidget.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_CLIENT_API UBJS_StateWidget : public UBJS_SubWidget
{
	GENERATED_BODY()

public:
	virtual void BJS_SubUpdateWidget() override;

	void SetAttack(int32 Attack);
	void SetSpeed(int32 Speed);

private:
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UTextBlock* tb_attack;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UTextBlock* tb_speed;
};
