// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "SkillStruct.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FSkillStruct : public FTableRowBase
{
	GENERATED_BODY()
public:
	/** 타입별 Value의 값이용 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(DisplayName="Type", MakeStructureDefaultValue="0"))
	int32 Type;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(DisplayName="Value", MakeStructureDefaultValue="0.000000"))
	double Value;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(DisplayName="Duration", MakeStructureDefaultValue="0.000000"))
	double Duration;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(DisplayName="Code", MakeStructureDefaultValue="0"))
	int32 Code;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(DisplayName="CoolTime", MakeStructureDefaultValue="0.000000"))
	double CoolTime;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(DisplayName="WeaponCode", MakeStructureDefaultValue="0"))
	int32 WeaponCode;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(DisplayName="Name"))
	FName Name;

	/** Image Path **/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(DisplayName="Path"))
	FName Path;

	FSkillStruct()
	{
		Type = 0;
		Value = 0;
		Duration = 0;
		Code = 0;
		CoolTime = 0;
		WeaponCode = 0;
	}

	FSkillStruct(const FSkillStruct& other)
	{
		Type = other.Type;
		Value = other.Value;
		Duration = other.Duration;
		Code = other.Code;
		CoolTime = other.CoolTime;
		WeaponCode = other.WeaponCode;
		Name = other.Name;
		Path = other.Path;
	}
};
