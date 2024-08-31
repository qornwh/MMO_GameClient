// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ItemEtcStruct.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FItemEtcStruct : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(DisplayName="Code", MakeStructureDefaultValue="0"))
	int32 Code;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(DisplayName="Type", MakeStructureDefaultValue="0"))
	int32 Type;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(DisplayName="Gold", MakeStructureDefaultValue="0"))
	int32 Gold;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(DisplayName="Size", MakeStructureDefaultValue="0"))
	int32 Size;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(DisplayName="Name", MakeStructureDefaultValue=""))
	FName Name;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(DisplayName="Path", MakeStructureDefaultValue=""))
	FName Path;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(DisplayName="Description", MakeStructureDefaultValue=""))
	FName Description;
};
