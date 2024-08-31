// Fill out your copyright notice in the Description page of Project Settings.


#include "BJS_Buffer.h"

BJS_Buffer::BJS_Buffer(int32 size)
{
	_buffer.Init(0, size);
	UE_LOG(LogTemp, Log, TEXT("_buffer init %d !!!"), _buffer.Num());
	_bufferSize = size;
}

BJS_Buffer::~BJS_Buffer()
{
}

void BJS_Buffer::Clean()
{
	int32 dataSize = DataSize();
	if (dataSize > 0)
		FMemory::Memcpy(&_buffer[0], &_buffer[_readPos], dataSize);

	_writePos -= _readPos;
	_readPos = 0;
}

bool BJS_Buffer::OnRead(int32 numOfBytes)
{
	if (numOfBytes > DataSize())
		return false;

	_readPos += numOfBytes;
	return true;
}

bool BJS_Buffer::OnWrite(int32 numOfBytes)
{
	if (numOfBytes > FreeSize())
		return false;

	_writePos += numOfBytes;
	return true;
}
