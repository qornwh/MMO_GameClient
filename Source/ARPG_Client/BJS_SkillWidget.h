// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BJS_SkillWidget.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_CLIENT_API UBJS_SkillWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetSkillIcon(UTexture2D* Image);
	void SetUpdateCoolTime(float CoolTime);

private:
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UImage* img_icon;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UTextBlock* tb_time;
};
