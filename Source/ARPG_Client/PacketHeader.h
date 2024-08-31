// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class PacketHeader
{
public:
	PacketHeader(uint16 id, uint16 size);
	~PacketHeader();
	
	uint16 GetId() const;
	void SetId(uint16 id);
	uint16 GetSize() const;
	void SetSize(uint16 size);

private:
	uint16 Id;
	uint16 Size;
};
