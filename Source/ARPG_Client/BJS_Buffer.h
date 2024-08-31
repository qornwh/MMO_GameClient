// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

using BJS_BufferPtr = TSharedPtr<class BJS_Buffer>;

/**
 * 
 */
class BJS_Buffer : public TSharedFromThis<BJS_Buffer>
{
public:
	BJS_Buffer(int32 size = 1024 * 100); // 102400 = 100k
	~BJS_Buffer();

	void Clean();
	bool OnRead(int32 numOfBytes);
	bool OnWrite(int32 numOfBytes);

	BYTE* ReadPos() { return &_buffer[_readPos]; }
	BYTE* WritePos() { return &_buffer[_writePos]; }
	int32 DataSize() { return _writePos - _readPos; }
	int32 FreeSize() { return _bufferSize - _writePos; };
	BYTE* Buffer() { return _buffer.GetData(); }

private:
	int32 _readPos = 0;
	int32 _writePos = 0;
	int32 _bufferSize = 0;
	TArray<BYTE> _buffer;
};