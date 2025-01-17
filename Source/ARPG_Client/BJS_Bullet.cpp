#include "BJS_Bullet.h"

#include "BJS_CharaterState.h"
#include "BJS_GameModeBase.h"
#include "BJS_InGameMode.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/MovementComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

ABJS_Bullet::ABJS_Bullet()
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collision"));
	CollisionComponent->InitCapsuleSize(10.f, 10.f);
	CollisionComponent->SetNotifyRigidBodyCollision(true);
	SetRootComponent(CollisionComponent);

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

void ABJS_Bullet::BeginPlay()
{
	Super::BeginPlay();
	SetActorTickInterval(InterVal);
}

void ABJS_Bullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AttackCheckTimer -= DeltaTime;
	if (AttackCheckTimer < 0.f)
		Destroy();
}

void ABJS_Bullet::InitStartDirection(FVector Direction, FVector Pos, int32 SkillCode, int32 AttackNumber)
{
	auto mode = Cast<ABJS_InGameMode>(GetWorld()->GetAuthGameMode());
	if (mode == nullptr) return;
	
	ProjectileMovementComponent->Velocity = Direction * ProjectileMovementComponent->InitialSpeed;
	PresentPos = GetActorLocation();
	_SkillCode = SkillCode;
	_AttackNumber = AttackNumber;
}

void ABJS_Bullet::ChangeDirection(FVector TargetDirection)
{
	FRotator lookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetDirection);
	FVector direction = lookAtRotation.Vector();
	ProjectileMovementComponent->Velocity = direction * ProjectileMovementComponent->InitialSpeed;
	lookAtRotation.Yaw += 90;
	SetActorRotation(lookAtRotation);
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

void ABJS_Bullet::SetSkillCode(int32 Code)
{
	_SkillCode = Code;
}

void ABJS_Bullet::SetState(TSharedPtr<BJS_CharaterState> State)
{
	CharaterState = State;
}

void ABJS_Bullet::SetPresentPos()
{
	PresentPos = GetActorLocation();
}

float ABJS_Bullet::GetCollisionWidth() const
{
	return CollisionComponent->GetScaledCapsuleHalfHeight() - CollisionComponent->GetScaledCapsuleRadius();
}

bool ABJS_Bullet::IsCoolDown()
{
	return AttackCheckTimer < 0.0f;
}
