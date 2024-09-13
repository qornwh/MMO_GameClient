// Fill out your copyright notice in the Description page of Project Settings.


#include "BJS_LoginPawn.h"

#include "BJS_WeaponActor.h"

ABJS_LoginPawn::ABJS_LoginPawn()
{
	CreateCamerCompoent();

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstance(TEXT("/Script/Engine.AnimBlueprint'/Game/MyGame/Assets/Charater/LoginMesh/ABP_LoginMeshAnim.ABP_LoginMeshAnim_C'"));
	check(AnimInstance.Succeeded());
	DefaultSkeletalMeshComponent->SetAnimInstanceClass(AnimInstance.Class);
}

void ABJS_LoginPawn::BeginPlay()
{
	Super::BeginPlay();
	FRotator rot(0,90,0);
	SetActorRotation(rot);
}

void ABJS_LoginPawn::Destroyed()
{
	Super::Destroyed();
}

void ABJS_LoginPawn::CreateWeaponActor(USkeletalMesh* WeaponMesh)
{
	FName WeaponSocket(TEXT("Weapon_R"));
	if (Pistol != nullptr)
		Pistol->Destroy();
	
	if (WeaponMesh && DefaultSkeletalMeshComponent->DoesSocketExist(WeaponSocket))
	{
		FVector loc(-9.f, 2.f, -2.f);
		FRotator rot(0.f, 90.f, 0.f);
		Pistol = GetWorld()->SpawnActor<ABJS_WeaponActor>(ABJS_WeaponActor::StaticClass());
		Pistol->SetSkeletalMesh(WeaponMesh);
		Pistol->AttachToComponent(DefaultSkeletalMeshComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket);
		Pistol->SetActorRelativeLocation(loc);
		Pistol->SetActorRelativeRotation(rot);
	}
}

void ABJS_LoginPawn::SetVisible(bool Flag)
{
	Super::SetVisible(Flag);
	if (Pistol)
		Pistol->SetActorHiddenInGame(!Flag);
}
