// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ExpLvStruct.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FExpLvStruct : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(DisplayName="Lv", MakeStructureDefaultValue="0"))
	int32 Lv;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(DisplayName="Exp", MakeStructureDefaultValue="0"))
	int32 Exp;
};
