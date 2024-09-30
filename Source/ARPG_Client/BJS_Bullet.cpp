// Fill out your copyright notice in the Description page of Project Settings.


#include "BJS_Bullet.h"

#include "BJS_CharaterState.h"
#include "BJS_GameInstance.h"
#include "BJS_GameModeBase.h"
#include "BJS_InGameMode.h"
#include "BJS_Monster.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/MovementComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ABJS_Bullet::ABJS_Bullet() : CurrentTarget(nullptr)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collision"));
	SetRootComponent(CollisionComponent);
	CollisionComponent->InitCapsuleSize(10.f, 10.f);
	CollisionComponent->SetNotifyRigidBodyCollision(true);

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMeshComponent->SetupAttachment(RootComponent);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
	ProjectileMovementComponent->InitialSpeed = Speed;
	ProjectileMovementComponent->MaxSpeed = Speed;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->bShouldBounce = false;
	ProjectileMovementComponent->Bounciness = 0.f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
}

// Called when the game starts or when spawned
void ABJS_Bullet::BeginPlay()
{
	Super::BeginPlay();

	if (!isMultipleAttack)
	{
		// 총알등 충돌 1번만 일어나는것만 체크
		CollisionComponent->OnComponentHit.AddDynamic(this, &ABJS_Bullet::OnHitEvent);
	}
}

void ABJS_Bullet::Destroyed()
{
	Super::Destroyed();
	UE_LOG(LogTemp, Log, TEXT("Bulet Destroy !!!"));
}

// Called every frame
void ABJS_Bullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsTargeting)
	{
		if (!CurrentTarget)
		{
			// 대상이 없어지면 탄알 제거
			Destroy();
		}
		else
		{
			ChangeDirection(CurrentTarget->GetActorLocation());
		}
	}
	if (isMultipleAttack)
	{
		HitCheck();
	}

	AttackTimer += DeltaTime;
	if (AttackTimer >= AttackCheckTimer)
		Destroy();
}

void ABJS_Bullet::InitStartDirection(FVector Direction, FVector Pos)
{
	ProjectileMovementComponent->Velocity = Direction * ProjectileMovementComponent->InitialSpeed;
	StartPos = Pos;
}

void ABJS_Bullet::ChangeDirection(FVector TargetDirection)
{
	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetDirection);
	FVector Direction = LookAtRotation.Vector();
	ProjectileMovementComponent->Velocity = Direction * ProjectileMovementComponent->InitialSpeed;
	LookAtRotation.Yaw += 90;
	SetActorRotation(LookAtRotation);
}

void ABJS_Bullet::OnHitEvent(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor)
	{
		// UE_LOG(LogTemp, Warning, TEXT("Hit Bullet with actor: %s %s!!!"), *OtherActor->GetName(), *CollisionComponent->GetCollisionProfileName().ToString());
		// UE_LOG(LogTemp, Warning, TEXT("Hit Bullet with actor: %s %s!!!"), *OtherActor->GetName(), *OtherComp->GetCollisionProfileName().ToString());
		auto BJSCharater = Cast<ABJS_Character>(OtherActor);
		if (BJSCharater)
		{
			auto mode = Cast<ABJS_InGameMode>(GetWorld()->GetAuthGameMode());
			if (BJSCharater->GetHitted())
			{
				if (Cast<ABJS_Monster>(BJSCharater))
				{
					// 나일때만 데미지 전달
					if(CharaterState.IsValid() && CharaterState.Pin()->GetUUid() == mode->GetMyState()->GetUUid())
					{
						mode->SetTakeDemageList(BJSCharater->GetState());
						FVector loc = BJSCharater->GetActorLocation();
						FRotator rot = BJSCharater->GetActorRotation();
						
						BJSCharater->GetState()->SetX(loc.X);
						BJSCharater->GetState()->SetY(loc.Y);
						BJSCharater->GetState()->SetZ(loc.Z);
						BJSCharater->GetState()->SetYaw(rot.Yaw);
						mode->TakeDemage(true, SkillCode, mode->GetMyState());
					}
				}
				else if (Cast<ABJS_ControlCharacter>(BJSCharater))
				{
					mode->SetTakeDemageList(mode->GetMyState());
					if (ParentTarget)
						mode->TakeDemage(false, SkillCode, ParentTarget->GetState());
				}
			}
		}
	}
	Destroy();
}

int32 ABJS_Bullet::GetHightSize()
{
	if (CollisionComponent)
	{
		return CollisionComponent->GetScaledCapsuleHalfHeight();
	}
	return 0;
}

int32 ABJS_Bullet::GetAttackTime()
{
	return AttackCheckTimer;
}

void ABJS_Bullet::HitCheck()
{
	TArray<AActor*> OverlapResults;
	CollisionComponent->GetOverlappingActors(OverlapResults);

	auto mode = Cast<ABJS_InGameMode>(GetWorld()->GetAuthGameMode());
	for (auto Actor : OverlapResults)
	{
		auto Monster = Cast<ABJS_Monster>(Actor);
		if (Actor && Actor != this && Monster)
		{
			if (CharaterState.IsValid() && CharaterState.Pin()->GetUUid() == mode->GetMyState()->GetUUid())
			{
				mode->SetTakeDemageList(Monster->GetState());
				FVector loc = Monster->GetActorLocation();
				FRotator rot = Monster->GetActorRotation();
				Monster->GetState()->SetX(loc.X);
				Monster->GetState()->SetY(loc.Y);
				Monster->GetState()->SetZ(loc.Z);
				Monster->GetState()->SetYaw(rot.Yaw);
			}
		}
	}
	mode->TakeDemage(true, SkillCode, mode->GetMyState());
}

void ABJS_Bullet::SetTarget(ABJS_Character* Target, ABJS_Character* Parent)
{
	// 일단 임시로 몬스터가 공격한거만 들고있음
	CurrentTarget = Target;
	ParentTarget = Parent;
	IsTargeting = true;
}

void ABJS_Bullet::SetSkillCode(int32 Code)
{
	SkillCode = Code;
}

void ABJS_Bullet::SetState(TSharedPtr<BJS_CharaterState> State)
{
	CharaterState = State;
}
