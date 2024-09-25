// Fill out your copyright notice in the Description page of Project Settings.


#include "FriendSystem.h"

FriendSystem::FriendSystem()
{
}

FriendSystem::~FriendSystem()
{
}

void FriendSystem::AddFriend(int32 friendCode, bool access, const FString& Name)
{
	if (!FriendList.Contains(friendCode))
	{
		FriendList.Add(friendCode, Person(friendCode, access, Name));
	}
}

void FriendSystem::RemoveFriend(int32 friendCode)
{
	if (FriendList.Contains(friendCode))
	{
		FriendList.Remove(friendCode);
	}
}

void FriendSystem::UpdateFriend(int32 friendCode, bool access)
{
	if (FriendList.Contains(friendCode))
	{
		auto& myFriend = FriendList[friendCode];
		myFriend.Access = access;
	}
}

int32 FriendSystem::CheckFriend(const FString& Name)
{
	for (const auto& entry : FriendList)
	{
		const auto& myFriend = entry.Value;

		if (myFriend.Name.Equals(Name))
		{
			return entry.Key;
		}
	}
	return -1;
}
