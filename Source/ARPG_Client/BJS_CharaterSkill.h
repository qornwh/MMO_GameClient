#pragma once

struct CharaterSkill
{
	// 비트 마스크 이용
	enum BUFFTYPES
	{
		HP = 1,
		MSP = 1 << 1,
		ASP = 1 << 2,
	};

	CharaterSkill(int32 SkillCode, float DurationTime, float CoolDownTime) : Code(SkillCode), Duration(DurationTime), CoolDown(CoolDownTime)
	{
	}

	void UpdateBuff(float Deltatime)
	{
		DurationRemaining -= Deltatime;
		CoolDownRemaining -= Deltatime;
		if (DurationRemaining <= 0)
			DurationRemaining = 0.f;
		if (CoolDownRemaining <= 0)
			CoolDownRemaining = 0.f;
	}

	void Start()
	{
		DurationRemaining = Duration;
		CoolDownRemaining = CoolDown;
		if (Duration > 0)
			Durationing = true;
	}

	bool IsDurationEnd()
	{
		if (Duration > 0 && Durationing)
		{
			if (DurationRemaining == 0)
			{
				Durationing = false;
				return true;
			}
		}
		return false;
	}

	float Duration = 0.f;
	float CoolDown = 0.f;
	float DurationRemaining = 0.f;
	float CoolDownRemaining = 0.f;
	bool Durationing = false;

	int32 Code = 0;
};
