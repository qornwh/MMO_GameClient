// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

struct Person
{
	Person() : PlayerCode(-1)
	{
	}

	Person(int32 PlayerCode, bool Access, FString Name) : PlayerCode(PlayerCode), Access(Access), Name(Name)
	{
	}

	Person(const Person& other) : PlayerCode(other.PlayerCode), Access(other.Access), Name(other.Name)
	{
	}

	bool operator==(const Person& other) const
	{
		if (PlayerCode == other.PlayerCode)
			return true;
		return false;
	}

	int32 PlayerCode = 0;
	bool Access = false;
	FString Name;
};

class ARPG_CLIENT_API FriendSystem
{
public:
	FriendSystem();
	~FriendSystem();

	void AddFriend(int32 friendCode, bool access, const FString& Name);
	void RemoveFriend(int32 friendCode);
	void UpdateFriend(int32 friendCode, bool access); // 접속여부

	TMap<int32, Person>& GetFriendList() { return FriendList; }

private:
	TMap<int32, Person> FriendList;
};
