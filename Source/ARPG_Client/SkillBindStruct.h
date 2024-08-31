// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "SkillBindStruct.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FSkillBindStruct : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(DisplayName="WeaponCode", MakeStructureDefaultValue="0"))
	int32 WeaponCode;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(DisplayName="SkillCode", MakeStructureDefaultValue="0"))
	int32 SkillCode;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(DisplayName="KeyCode", MakeStructureDefaultValue="0"))
	int32 KeyCode;
};
