#include "BJS_Monster.h"

#include "BJS_Bullet.h"
#include "BJS_CharaterState.h"
#include "BJS_GameInstance.h"
#include "BJS_InGameMode.h"
#include "Components/CapsuleComponent.h"

ABJS_Monster::ABJS_Monster()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	GetCapsuleComponent()->InitCapsuleSize(40.f, 96.0f);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Monster"));
	GetCapsuleComponent()->SetNotifyRigidBodyCollision(true);

	MonsterMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MonsterMeshComponent"));
	MonsterMeshComponent->SetupAttachment(RootComponent);
}

void ABJS_Monster::BeginPlay()
{
	Super::BeginPlay();
	
	auto instance = Cast<UBJS_GameInstance>(GetGameInstance());
	if (instance)
	{
		BulletClass = instance->GetMonsterSkillClass();		
		MonsterMeshList = instance->GetMonsterMeshList();

		MonsterMeshComponent->SetStaticMesh(MonsterMeshList[1]);
	}
	
	IsHitted = true;
}

void ABJS_Monster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABJS_Monster::Move(float DeltaTime)
{
	// Super::Move(DeltaTime);
	
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

	if (!cur.Equals(dest))
	{
		FVector next = cur + (dest - cur).GetSafeNormal() * DeltaTime * State->GetSpeed();
		if (FVector::Dist(next, dest) <= State->GetSpeed() * DeltaTime)
		{
			next = dest;
		}
		SetActorLocation(next);
	}
}

void ABJS_Monster::OnHitEvent(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	FName presetName("Bullet");

	if (OtherComp && OtherActor)
	{
		if (OtherComp->GetCollisionProfileName().IsEqual(presetName))
		{
			UE_LOG(LogTemp, Log, TEXT("OnHit Monster !!!"));
		}
	}
}

void ABJS_Monster::SetState(TSharedPtr<BJS_CharaterState> state)
{
	Super::SetState(state);

	int32 MonsterTyep = state->GetCode();
	if (MonsterMeshList.Contains(MonsterTyep))
	{
		MonsterMeshComponent->SetStaticMesh(MonsterMeshList[MonsterTyep]);
	}
}

void ABJS_Monster::Attack()
{
	// Super::Attack();

	int32 SkillCode = 0;
	FVector SpawnLocation = GetActorLocation();
	// auto Bullet = GetWorld()->SpawnActor<ABJS_Bullet>(BulletClass, SpawnLocation, FRotator::ZeroRotator);
	// Bullet->InitStartDirection(GetActorForwardVector(), SpawnLocation);
	// Bullet->SetSkillCode(SkillCode);
	// if (TargetUUid >= 0)
	// {
	// 	auto instance = Cast<ABJS_InGameMode>(GetWorld()->GetAuthGameMode());
	// 	if (instance)
	// 	{
	// 		if (instance->GetCharaterStateList().Contains(TargetUUid))
	// 		{
	// 			auto BJS_Target = instance->GetCharaterStateList()[TargetUUid];
	// 			if (BJS_Target && BJS_Target->GetTarget())
	// 			{
	// 				Bullet->SetTarget(BJS_Target->GetTarget(), this);
	// 				UE_LOG(LogTemp, Log,  TEXT("Spawn Bullet !!!!"));
	// 			}
	// 		}
	// 	}
	// }
}

void ABJS_Monster::AttackEnd()
{
	// Super::AttackEnd();
}

