// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "WeaponStruct.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType) 
struct FWeaponStruct : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(DisplayName="Code", MakeStructureDefaultValue="0"))
	int32 Code;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(DisplayName="AttackSpeed", MakeStructureDefaultValue="0.000000"))
	double AttackSpeed;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(DisplayName="Name"))
	FName Name;
};
