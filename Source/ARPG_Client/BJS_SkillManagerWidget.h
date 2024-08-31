// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BJS_SubWidget.h"
#include "BJS_SkillManagerWidget.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_CLIENT_API UBJS_SkillManagerWidget : public UBJS_SubWidget
{
	GENERATED_BODY()

public:
	virtual void BJS_InitWidget() override;
	virtual void BJS_SubUpdateWidget() override;

	void CoolTimeUpdate(int32 KeyBind, float CoolTime);

private:
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UBJS_SkillWidget* skill_1;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UBJS_SkillWidget* skill_2;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UBJS_SkillWidget* skill_3;
};
