// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

struct BJS_ExState
{
	void AddAttack(int32 Value);
	void AddSpeed(float Value);
	void AddDeffence(int32 Value);
	
	// state
	int32 Attack = 0;
	float Speed = 0;
	int32 Deffence = 0;
};

/**
 * 
 */
class ARPG_CLIENT_API BJS_CharaterState : public TSharedFromThis<BJS_CharaterState>
{
public:
	
	enum CharaterState
	{
		IDLE = 0,
		DIE = 1,
		HITED = 2,
		MOVE = 3,
		ATTACK = 4,
		SKILL1 = 5,
		SKILL2 = 6,
		HEAL = 7
	};
	
	BJS_CharaterState();
	~BJS_CharaterState();
	
	int32 GetLv() const;
	void SetLv(int32 lv);
	FString GetName() const;
	void SetName(const FString& name);
	int32 GetExp() const;
	void SetExp(int32 exp);
	int32 GetGold() const;
	void SetGold(int32 gold);
	int32 GetMaxHp() const;
	void SetMaxHp(int32 maxHp);
	int32 GetHp() const;
	void SetHp(int32 hp);
	int32 GetAttack() const;
	void SetAttack(int32 Attack);
	float GetSpeed() const;
	void SetSpeed(float Speed);
	int32 GetDeffence() const;
	void SetDeffence(int32 Deffence);
	void SetState(int32 state);
	int32 GetState();
	int32 GetX() const;
	void SetX(int32 x);
	int32 GetY() const;
	void SetY(int32 y);
	int32 GetZ() const;
	void SetZ(int32 z);
	int32 GetYaw() const;
	void SetYaw(int32 yaw);
	int32 GetUUid() const;
	void SetUUid(int32 uUid);
	int32 GetCode() const;
	void SetCode(int32 code);
	int32 GetWeaponCode() const;
	void SetWeaponCode(int32 weaponCode);
	
	void SetTarget(class ABJS_Character* target);
	class ABJS_Character* GetTarget();

	void SetIsMonster(bool isMonster);
	bool GetIsMonster();

	// 버프의 스텟도 여기에 둔다
	BJS_ExState BuffState;

	// 아이템의 스텟도 여기에 둔다
	BJS_ExState ItemState;
	
private:
	bool IsMonster = false;
	int32 Lv;
	FString Name;
	int32 Exp;
	int32 Gold;
	int32 MaxHp;
	int32 Hp;

	// state
	int32 Attack;
	float Speed;
	int32 Deffence;
	int32 State;

	// postion
	int32 X;
	int32 Y;
	int32 Z;
	int32 Yaw;

	// 장비
	int32 UUid;
	int32 Code;
	int32 WeaponCode;

	// 해당하는 액터 포인터 들고 있는다.
	class ABJS_Character* Target;
};
