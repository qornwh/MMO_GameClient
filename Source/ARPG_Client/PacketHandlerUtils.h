// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <google/protobuf/message.h>
#include "CoreMinimal.h"
#include "BJS_Buffer.h"
#include "PacketHeader.h"

/**
 * 
 */
class PacketHandlerUtils
{
public:
	static bool CheckPacketHeader(PacketHeader* header, int32 offset, int32 len)
	{
		if (header == nullptr)
			return false;

		int dataSize = len - offset;

		if (dataSize < 4)
			return false;

		if (dataSize < header->GetSize())
			return false;

		return true;
	}
    
	static bool ParsePacketHandler(google::protobuf::Message& pkt, BYTE* buffer, int32 dataSize, int32& offset)
	{
		const BYTE* payloadPtr = &buffer[offset];
		const bool parseResult = pkt.ParseFromArray(payloadPtr, dataSize);
		if (parseResult)
		{
			offset += static_cast<int32>(pkt.ByteSizeLong());
			return true;
		}
		return false;
	}
	
	static void MakeHeaderPacketHandler(BJS_BufferPtr buffer, uint16 pktId, uint16 pktSize)
	{
		PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer->ReadPos());
		header->SetId(pktId);
		header->SetSize(pktSize + sizeof(PacketHeader));
		buffer->OnWrite(sizeof(PacketHeader));
	}

	static void MakePacketHandler(BJS_BufferPtr buffer, google::protobuf::Message& pkt, uint16 pktId)
	{
		uint16 pktSize = pkt.ByteSizeLong();
		MakeHeaderPacketHandler(buffer, pktId, pktSize);
		if (pktSize > 0)
		{
			pkt.SerializeToArray(&buffer->ReadPos()[sizeof(PacketHeader)], pktSize);
			buffer->OnWrite(pktSize);
		}
	}
};
