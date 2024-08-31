// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "PlayerStruct.generated.h"

/**
 * 
 */
/** Please add a struct description */
USTRUCT(BlueprintType)
struct FPlayerStruct : public FTableRowBase
{
	GENERATED_BODY()
public:
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(DisplayName="Code", MakeStructureDefaultValue="0"))
	int32 Code;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(DisplayName="Attack", MakeStructureDefaultValue="0.000000"))
	double Attack;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(DisplayName="MoveSpeed", MakeStructureDefaultValue="0.000000"))
	double MoveSpeed;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(DisplayName="Hp", MakeStructureDefaultValue="0.000000"))
	double Hp;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(DisplayName="Name"))
	FName Name;
};
