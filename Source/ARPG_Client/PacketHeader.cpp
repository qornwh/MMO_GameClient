// Fill out your copyright notice in the Description page of Project Settings.


#include "PacketHeader.h"

PacketHeader::PacketHeader(uint16 id, uint16 size) : Id(id), Size(size)
{
}

PacketHeader::~PacketHeader()
{
}

uint16 PacketHeader::GetId() const
{
	return Id;
}

void PacketHeader::SetId(uint16 id)
{
	Id = id;
}

uint16 PacketHeader::GetSize() const
{
	return Size;
}

void PacketHeader::SetSize(uint16 size)
{
	Size = size;
}
