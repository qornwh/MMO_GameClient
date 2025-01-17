// Fill out your copyright notice in the Description page of Project Settings.


#include "BJS_Character.h"

#include "BJS_AnimInstance_Base.h"
#include "BJS_BuffSkill.h"
#include "BJS_Bullet.h"
#include "BJS_CharaterSkill.h"
#include "BJS_CharaterUIWidget.h"
#include "BJS_WeaponActor.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraSystem.h"
#include "BJS_CharaterState.h"
#include "BJS_DemageActor.h"
#include "BJS_GameInstance.h"
#include "BJS_GameModeBase.h"
#include "SkillBindStruct.h"
#include "SkillStruct.h"
#include "GameFramework/CharacterMovementComponent.h"

ABJS_Character::ABJS_Character()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->GravityScale = 1.75f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;

	// 애니메이션 설정
	static ConstructorHelpers::FClassFinder<UBJS_AnimInstance_Base> MoveAnimInstanceClass(TEXT("/Script/Engine.AnimBlueprint'/Game/MyGame/Assets/Charater/Animation/ABP_Move.ABP_Move_C'"));
	check(MoveAnimInstanceClass.Succeeded());
	AnimInstanceClass = MoveAnimInstanceClass.Class;

	// Demage는 블루프린트화 안하고 바로 C++붙임
	static ConstructorHelpers::FClassFinder<ABJS_DemageActor> DamageActorClass(TEXT("/Script/CoreUObject.Class'/Script/ARPG_Client.BJS_DemageActor'"));
	check(DamageActorClass.Succeeded());
	DefaultDamgeActor = DamageActorClass.Class;

	// 머리위에 UI 띄우기
	static ConstructorHelpers::FClassFinder<UUserWidget> UpBarClass(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/MyGame/UMG/BJS_WBP_ChararterUI.BJS_WBP_ChararterUI_C'"));
	check(UpBarClass.Succeeded());
	HpBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBAR"));
	HpBar->SetupAttachment(GetCapsuleComponent());
	HpBar->SetRelativeLocation(FVector(0.f, 0.f, 100.f));
	HpBar->SetWidgetSpace(EWidgetSpace::World);
	HpBar->SetWidgetClass(UpBarClass.Class);
	HpBar->SetDrawSize(FVector2D(200.f, 60.f));
	HpBar->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));
}

void ABJS_Character::BeginPlay()
{
	Super::BeginPlay();

	auto instance = Cast<UBJS_GameInstance>(GetGameInstance());
	if (instance)
	{
		CharaterMeshList = instance->GetCharaterMeshList();
		WeaponMeshList = instance->GetWeaponMeshList();
		FireStartFXList = instance->GetFireStartFX();
		BulletFXList = instance->GetNiagaraSkillMap();
	}

	auto mode = Cast<UBJS_GameInstance>(GetGameInstance());
	if (mode)
	{
		BulletClass = mode->GetSkillBulletMap()[0];		
	}
}

void ABJS_Character::SetSkeletalMesh(USkeletalMesh* CurrentMesh)
{
	check(CurrentMesh != nullptr);
	GetMesh()->SetSkeletalMesh(CurrentMesh);
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -98.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
}

void ABJS_Character::Move(float DeltaTime)
{
	FRotator roc = GetActorRotation();
	if (roc.Yaw != State->GetYaw())
	{
		roc.Yaw = State->GetYaw();
		SetActorRotation(roc);
	}

	FVector cur = GetActorLocation();
	FVector dest = GetActorLocation();
	dest.X = State->GetX();
	dest.Y = State->GetY();

	if (!cur.Equals(dest, 20))
	{
		FVector direction = (dest - cur).GetSafeNormal();
		if (FVector::Dist(cur, dest) < DeltaTime * State->GetSpeed())
		{
			// 가야되는 거리보다 움직이는 거리가 커지면 그냥 set
			SetActorLocation(dest);
		}
		else
		{
			AddMovementInput(direction, 1.0f, true);
		}
	}
	else
	{
		// 일단 지근거리면 그냥 set
		SetActorLocation(dest);
	}
}

void ABJS_Character::Destroyed()
{
	Super::Destroyed();

	if (Weapon.Get() != nullptr)
	{
		Weapon->Destroy();
	}
}

void ABJS_Character::UpdateBuff(float DeltaTime)
{
	auto instance = Cast<UBJS_GameInstance>(GetWorld()->GetGameInstance());
	for (auto& buff : BuffList)
	{
		buff.Value.UpdateBuff(DeltaTime);
		if (buff.Value.IsDurationEnd())
		{
			if (instance && instance->GetSkillStructs().Contains(buff.Value.Code))
			{
				auto skill = instance->GetSkillStructs()[buff.Value.Code];
				int32 Type = skill->Type;
				float Value = skill->Value;

				if (Type == CharaterSkill::SKILLTYPES::MSP)
				{
					State->BuffState.AddSpeed(-Value);
				}
			}
		}
	}
}

void ABJS_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateHpWidget();
	UpdateBuff(DeltaTime);
	Move(DeltaTime);
	SetAttackTimer(DeltaTime);
}

// Called to bind functionality to input
void ABJS_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

bool ABJS_Character::GetAim()
{
	return IsAim;
}

void ABJS_Character::SetAim(bool flag)
{
	IsAim = flag;
}

void ABJS_Character::SetAttackTimer(float DeltaTime)
{
	// 총 쏘는 애니메이션 종료는 같이쓴다(나, 다른플레이어)
	if (AttackTimer < 0)
		return;
	AttackTimer += DeltaTime;
	if (AttackTimer >= AttackCheckTime)
	{
		AttackTimer = -1;
		Weapon->FireEnd(nullptr);
	}
}

void ABJS_Character::StopJump()
{
	StopJumping();
}

void ABJS_Character::StartJump()
{
	Jump();
}

void ABJS_Character::CreateWeaponActor(USkeletalMesh* WeaponMesh)
{
	if (WeaponMesh)
	{
		Weapon = GetWorld()->SpawnActor<ABJS_WeaponActor>(ABJS_WeaponActor::StaticClass());
		Weapon->SetSkeletalMesh(WeaponMesh);
		ChangeWeaponSocket();
		Weapon->OnAttackEnd.BindUObject(this, &ABJS_Character::AttackEnd);
		Weapon->SetState(State);
	}
}

void ABJS_Character::ChangeWeaponSocket()
{
	if (!Weapon) return;
	FVector loc(0.f, 0.f, 0.f);
	FRotator rot(0.f, 90.f, 0.f);
	FName WeaponSocket(TEXT("Weapon_S3"));
	if (GetAim())
	{
		WeaponSocket = TEXT("Weapon_R");
		rot.Yaw = 0.f;
	}
	if (GetMesh()->DoesSocketExist(WeaponSocket))
	{
		Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket);
		Weapon->SetActorRelativeLocation(loc);
		Weapon->SetActorRelativeRotation(rot);
		Weapon->SetActorHiddenInGame(false);
	}
	else
	{
		Weapon->SetActorHiddenInGame(true);
	}
}

void ABJS_Character::SetMuzzleFX(UParticleSystem* MuzzleFX)
{
	if (Weapon && MuzzleFX)
	{
		Weapon->SetMuzzleFX(MuzzleFX);
	}
}

void ABJS_Character::SetBulletFX(UNiagaraSystem* SkillFx, int32 Code)
{
	if (Weapon && SkillFx)
	{
		Weapon->SetBulletFX(SkillFx, Code);
	}
}

void ABJS_Character::Attack()
{
}

void ABJS_Character::AttackEnd()
{
	if (AnimInstance)
	{
		AnimInstance->StopAttackAM();
		IsAttack = true;
	}
}

void ABJS_Character::PlayMotion(int32 Code, bool ignore)
{
	if (Code == 0)
	{
		PlayAttack(Code, ignore);
	}
	else
	{
		PlaySkill(Code, ignore);
	}
}

void ABJS_Character::PlayAttack(int32 Code, bool ignore)
{
	// 공격 스킬
	if (IsAim && Weapon)
	{
		IsAttack = false;
		if (Weapon->IsFire() && !ignore)
		{
			return;
		}
		Weapon->FireStart();
		if (AnimInstance)
			AnimInstance->PlayAttackAM();
	}

	FVector SpawnLocation = Weapon->GetSpawnLocation();
	auto mode = Cast<UBJS_GameInstance>(GetGameInstance());

	ABJS_Bullet* Bullet = nullptr;
	if (Code == 0)
	{
		Bullet = GetWorld()->SpawnActor<ABJS_Bullet>(BulletClass, SpawnLocation, FRotator::ZeroRotator);
		Bullet->InitStartDirection(GetActorForwardVector(), SpawnLocation, Code, PresentAttackNumber);
		Bullet->SetState(State);
		AttackTimer = 0;
		AttackCheckTime = 0.25f;
		if (PresentAttackNumber >= 0)
			Bullets.Add(PresentAttackNumber, Bullet);
	}
	else
	{
		Bullet = GetWorld()->SpawnActor<ABJS_Bullet>(mode->GetSkillBulletMap()[Code], SpawnLocation, FRotator::ZeroRotator);
		Bullet->InitStartDirection(FVector::ZeroVector, SpawnLocation, Code, PresentAttackNumber);
		Bullet->SetActorRelativeRotation(FRotator(90, 0, 0));
		Bullet->SetActorRelativeLocation(FVector(Bullet->GetHightSize(), 0, 0));
		Bullet->SetState(State);
	}
	if (PresentAttackNumber >= 0 && Bullet != nullptr)
		Bullets.Add(PresentAttackNumber, Bullet);
}

void ABJS_Character::PlaySkill(int32 Code, bool ignore)
{
	auto instance = Cast<UBJS_GameInstance>(GetWorld()->GetGameInstance());
	if (instance)
	{
		auto skill = instance->GetSkillStructs()[Code];
		float Duration = skill->Duration;
		float CoolTime = skill->CoolTime;
		int32 Type = skill->Type;
		float Value = skill->Value;

		if (Type == CharaterSkill::SKILLTYPES::MSP)
		{
			State->BuffState.AddSpeed(Value);
		}

		if (IsAim && Weapon && Type == 0)
		{
			// 공격 스킬
			IsAttack = false;
			if (Weapon->IsFire() && !ignore)
			{
				return;
			}

			Weapon->FireStart();
			if (AnimInstance)
				AnimInstance->PlayAttackAM();
		}
		else if (Type > 0)
		{
			// 버프 스킬
			ABJS_BuffSkill* BuffActor = GetWorld()->SpawnActor<ABJS_BuffSkill>(instance->GetBuffSkillClass());
			BuffActor->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
			BuffActor->SetBuffInit(instance->GetParticleSkillMap()[Code], Duration);
		}

		if (BuffList.Contains(Code))
		{
			BuffList[Code].Start();
		}
	}
}

TSharedPtr<BJS_CharaterState> ABJS_Character::GetState()
{
	return State;
}

void ABJS_Character::SetState(TSharedPtr<BJS_CharaterState> state)
{
	if (state == nullptr)
	{
		State.Reset();
		return;
	}

	State = state;
	const auto HpWidget = Cast<UBJS_CharaterUIWidget>(HpBar->GetUserWidgetObject());
	if (HpWidget != nullptr)
	{
		HpWidget->SetName(state->GetName());
		HpWidget->SetHp(state->GetHp() / state->GetMaxHp());
	}

	state->SetTarget(this);
	GetCharacterMovement()->MaxWalkSpeed = state->GetSpeed();
}

void ABJS_Character::TakeDemage(float Demage, ABJS_Character* Target)
{
	auto PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	auto CameraActor = Cast<AActor>(PlayerController->GetViewTarget());
	if (CameraActor)
	{
		FVector CameraLocation;
		FRotator CameraRotation;
		PlayerController->GetPlayerViewPoint(CameraLocation, CameraRotation);
		CameraRotation.Yaw += 180;
		CameraRotation.Pitch *= -1;

		FVector Loc = GetActorLocation() + (CameraActor->GetActorForwardVector() * -1 * 20);
		auto mode = Cast<ABJS_GameModeBase>(GetWorld()->GetAuthGameMode());
		if (mode)
		{
			ABJS_DemageActor* DemageActor = mode->CustomSpawnActor<ABJS_DemageActor>(DefaultDamgeActor, Loc, CameraRotation);
			DemageActor->SetDemage(Demage);
		}
	}
}

void ABJS_Character::TakeHeal(float Heal)
{
	auto PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	auto CameraActor = Cast<AActor>(PlayerController->GetViewTarget());
	if (CameraActor)
	{
		FVector CameraLocation;
		FRotator CameraRotation;
		PlayerController->GetPlayerViewPoint(CameraLocation, CameraRotation);
		CameraRotation.Yaw += 180;
		CameraRotation.Pitch *= -1;

		FVector Loc = GetActorLocation() + (CameraActor->GetActorForwardVector() * -1 * 20);
		auto mode = Cast<ABJS_GameModeBase>(GetWorld()->GetAuthGameMode());
		if (mode)
		{
			ABJS_DemageActor* DemageActor = mode->CustomSpawnActor<ABJS_DemageActor>(DefaultDamgeActor, Loc, CameraRotation);
			DemageActor->SetHeal(Heal);
		}
	}
}

bool ABJS_Character::GetHitted()
{
	return IsHitted;
}

void ABJS_Character::UpdateHpWidget()
{
	if (!HpBar) return;

	const auto HpWidget = Cast<UBJS_CharaterUIWidget>(HpBar->GetUserWidgetObject());
	if (HpWidget != nullptr)
	{
		auto PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		FVector ActorLocation = GetActorLocation();
		FVector CameraLocation;
		FRotator CameraRotation;
		PlayerController->GetPlayerViewPoint(CameraLocation, CameraRotation);
		CameraRotation.Yaw += 180;
		HpBar->SetWorldRotation(CameraRotation);

		auto dist = FVector::Dist(ActorLocation, CameraLocation);
		HpWidget->SetHp(static_cast<float>(State->GetHp()) / State->GetMaxHp());
		if (dist > 3000.f)
		{
			HpWidget->SetVisibility(ESlateVisibility::Hidden);
		}
		else
		{
			HpWidget->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void ABJS_Character::LoadInfo(int32 MeshCode, int32 WeaponCode)
{
	SetSkeletalMesh(CharaterMeshList[MeshCode]);
	CreateWeaponActor(WeaponMeshList[WeaponCode]);
	SetMuzzleFX(FireStartFXList);
	// 나이아가라 파티클, 충돌 따로임
	auto instance = Cast<UBJS_GameInstance>(GetGameInstance());
	if (instance)
	{
		auto& skillBinds = instance->GetSkillBindStructs()[WeaponCode];
		auto skillStruct = instance->GetSkillStructs();
		for (auto skillBind : skillBinds)
		{
			int32 keyBind = skillBind->KeyCode;
			int32 skillCode = skillBind->SkillCode;
			float duration = skillStruct[skillCode]->Duration;
			float cooltime = skillStruct[skillCode]->CoolTime;
			SkillList.Add(keyBind, skillCode);
			if (skillBind->KeyCode == 1)
			{
				// 일단 임시
				SetBulletFX(BulletFXList[skillCode], skillCode);
			}

			// 스킬 초기화
			if (BuffList.Contains(skillCode))
			{
				BuffList.Remove(skillCode);
			}
			BuffList.Add(skillCode, CharaterSkill{skillCode, duration, cooltime});
		}

		GetMesh()->SetAnimInstanceClass(AnimInstanceClass);
		AnimInstance = Cast<UBJS_AnimInstance_Base>(GetMesh()->GetAnimInstance());
	}
}

void ABJS_Character::SetActivate(bool Flag)
{
	if (Flag)
	{
		SetActorEnableCollision(true);
		SetActorTickEnabled(true);
		SetActorHiddenInGame(false);
	}
	else
	{
		SetActorEnableCollision(false);
		SetActorTickEnabled(false);
		SetActorHiddenInGame(true);
		SetState(nullptr);
	}
}
