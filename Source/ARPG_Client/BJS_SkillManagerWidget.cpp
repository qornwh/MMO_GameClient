// Fill out your copyright notice in the Description page of Project Settings.


#include "BJS_SkillManagerWidget.h"

#include "BJS_GameInstance.h"
#include "BJS_SkillWidget.h"
#include "SkillBindStruct.h"

void UBJS_SkillManagerWidget::BJS_InitWidget()
{
	Super::BJS_InitWidget();

	// 스킬아이콘 이미지 셋팅
	auto instance = Cast<UBJS_GameInstance>(GetGameInstance());
	if (instance)
	{
		int32 WeaponCode = instance->GetCurWeaponType();
		auto skillBinds = instance->GetSkillBindStructs()[WeaponCode];
		auto skillIcons = instance->GetSkillIconImgMap();

		for (auto entry : skillBinds)
		{
			auto img = skillIcons[entry->SkillCode];
			if (img)
			{
				if (entry->KeyCode == 1)
				{
					skill_1->SetSkillIcon(img);
				}
				else if (entry->KeyCode == 2)
				{
					skill_2->SetSkillIcon(img);
				}
				else if (entry->KeyCode == 3)
				{
					skill_3->SetSkillIcon(img);
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Not Find SKill %d %d !!!"), entry->KeyCode, entry->SkillCode);
			}
		}
	}
}

void UBJS_SkillManagerWidget::BJS_SubUpdateWidget()
{
	Super::BJS_SubUpdateWidget();
}

void UBJS_SkillManagerWidget::CoolTimeUpdate(int32 KeyBind, float CoolTime)
{
	// 이거는 어떻게 할수 없다 ifelse로 밖에...
	if (KeyBind == 1)
	{
		skill_1->SetUpdateCoolTime(CoolTime);
	}
	else if (KeyBind == 2)
	{
		skill_2->SetUpdateCoolTime(CoolTime);
	}
	else if (KeyBind == 3)
	{
		skill_3->SetUpdateCoolTime(CoolTime);
	}
}
