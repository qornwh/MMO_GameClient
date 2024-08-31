// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ItemEquipStruct.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FItemEquipStruct : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(DisplayName="Code", MakeStructureDefaultValue="0"))
	int32 Code;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(DisplayName="Type", MakeStructureDefaultValue="0"))
	int32 Type;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(DisplayName="Gold", MakeStructureDefaultValue="0"))
	int32 Gold;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(DisplayName="Attack", MakeStructureDefaultValue="0"))
	int32 Attack;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(DisplayName="Speed", MakeStructureDefaultValue="0"))
	int32 Speed;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(DisplayName="Name", MakeStructureDefaultValue=""))
	FName Name;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(DisplayName="Path", MakeStructureDefaultValue=""))
	FName Path;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(DisplayName="Description", MakeStructureDefaultValue=""))
	FName Description;
};
