// Fill out your copyright notice in the Description page of Project Settings.


#include "BJS_CharaterState.h"

void BJS_BuffState::AddAttack(int32 Value)
{
	Attack += Value;
}

void BJS_BuffState::AddSpeed(float Value)
{
	Speed += Value;
}

void BJS_BuffState::AddDeffence(int32 Value)
{
	Deffence += Value;
}

BJS_CharaterState::BJS_CharaterState(): Lv(0), Exp(0), WeaponCode(0), Target(nullptr), Gold(0), MaxHp(0), Hp(0), Attack(0), Speed(0), Deffence(0), State(0),
                                        X(0), Y(0), Z(0), Yaw(0), UUid(0), Code(0)
{
}

BJS_CharaterState::~BJS_CharaterState()
{
}

int32 BJS_CharaterState::GetLv() const
{
	return Lv;
}

void BJS_CharaterState::SetLv(int32 lv)
{
	Lv = lv;
}

FString BJS_CharaterState::GetName() const
{
	return Name;
}

void BJS_CharaterState::SetName(const FString& name)
{
	Name = name;
}

int32 BJS_CharaterState::GetExp() const
{
	return Exp;
}

void BJS_CharaterState::SetExp(int32 exp)
{
	Exp = exp;
}

int32 BJS_CharaterState::GetGold() const
{
	return Gold;
}

void BJS_CharaterState::SetGold(int32 gold)
{
	Gold = gold;
}

int32 BJS_CharaterState::GetMaxHp() const
{
	return MaxHp;
}

void BJS_CharaterState::SetMaxHp(int32 maxHp)
{
	MaxHp = maxHp;
}

int32 BJS_CharaterState::GetHp() const
{
	return Hp;
}

void BJS_CharaterState::SetHp(int32 hp)
{
	Hp = hp;
}

int32 BJS_CharaterState::GetAttack() const
{
	return Attack + BuffState.Attack;
}

void BJS_CharaterState::SetAttack(int32 attack)
{
	Attack = attack;
}

float BJS_CharaterState::GetSpeed() const
{
	return Speed + BuffState.Speed;
}

void BJS_CharaterState::SetSpeed(float speed)
{
	Speed = speed;
}

int32 BJS_CharaterState::GetDeffence() const
{
	return Deffence + BuffState.Deffence;
}

void BJS_CharaterState::SetDeffence(int32 deffence)
{
	Deffence = deffence;
}

void BJS_CharaterState::SetState(int32 state)
{
	State = state;
}

int32 BJS_CharaterState::GetState()
{
	return State;
}

int32 BJS_CharaterState::GetX() const
{
	return X;
}

void BJS_CharaterState::SetX(int32 x)
{
	X = x;
}

int32 BJS_CharaterState::GetY() const
{
	return Y;
}

void BJS_CharaterState::SetY(int32 y)
{
	Y = y;
}

int32 BJS_CharaterState::GetZ() const
{
	return Z;
}

void BJS_CharaterState::SetZ(int32 z)
{
	Z = z;
}

int32 BJS_CharaterState::GetYaw() const
{
	return Yaw;
}

void BJS_CharaterState::SetYaw(int32 yaw)
{
	Yaw = yaw;
}

int32 BJS_CharaterState::GetUUid() const
{
	return UUid;
}

void BJS_CharaterState::SetUUid(int32 uUid)
{
	UUid = uUid;
}

int32 BJS_CharaterState::GetCode() const
{
	return Code;
}

void BJS_CharaterState::SetCode(int32 code)
{
	Code = code;
}

int32 BJS_CharaterState::GetWeaponCode() const
{
	return WeaponCode;
}

void BJS_CharaterState::SetWeaponCode(int32 weaponCode)
{
	WeaponCode = weaponCode;
}

void BJS_CharaterState::SetTarget(ABJS_Character* target)
{
	Target = target;
}

ABJS_Character* BJS_CharaterState::GetTarget()
{
	return Target;
}

void BJS_CharaterState::SetIsMonster(bool isMonster)
{
	IsMonster = isMonster;
}

bool BJS_CharaterState::GetIsMonster()
{
	return IsMonster;
}
