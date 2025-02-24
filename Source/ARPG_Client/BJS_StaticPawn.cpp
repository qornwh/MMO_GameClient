// Fill out your copyright notice in the Description page of Project Settings.


#include "BJS_StaticPawn.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/MovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
ABJS_StaticPawn::ABJS_StaticPawn()
{
	// 조건 1. 콜라이더는 생성하지 않도록 한다.
	// 조건 2. 카메라는 생성 디폴트로 생성 하지 않는다.
	// 조건 3. 카메라, 스프링암 각도 위치 조작은 객체를 넘겨준다.
	// 조건 4. 메시 컴포넌트만 디폴트로 생성한다.
	PrimaryActorTick.bCanEverTick = false;

	auto CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
	CapsuleComponent->InitCapsuleSize(42.f, 96.0f);
	CapsuleComponent->SetMobility(EComponentMobility::Type::Movable);
	RootComponent = CapsuleComponent;
	
	DefaultSkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("DefaultSkeletalMeshComp"));
	DefaultSkeletalMeshComponent->SetupAttachment(RootComponent);
	
	DefaultMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DefaultStaticMeshComp"));
	DefaultMeshComponent->SetupAttachment(RootComponent);
	DefaultMeshComponent->SetMobility(EComponentMobility::Type::Static);
}

// Called when the game starts or when spawned
void ABJS_StaticPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABJS_StaticPawn::CreateCamerCompoent()
{
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 250.0f;
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->SetRelativeLocation(FVector(0,0,80.f));

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
}

void ABJS_StaticPawn::SetSkeletalMesh(USkeletalMesh* Mesh)
{
	if (!DefaultSkeletalMeshComponent)
	{
		return;
	}
		
	DefaultSkeletalMeshComponent->SetSkeletalMesh(Mesh);
}

void ABJS_StaticPawn::SetStaticMesh(UStaticMesh* Mesh)
{
	if (!DefaultMeshComponent)
	{
		return;
	}

	DefaultMeshComponent->SetStaticMesh(Mesh);
}

void ABJS_StaticPawn::SetVisible(bool Flag)
{
	SetActorHiddenInGame(!Flag);
}

USpringArmComponent* ABJS_StaticPawn::GetSpringArmComponent()
{
	return CameraBoom;
}

UCameraComponent* ABJS_StaticPawn::GetFollowCamera()
{
	return FollowCamera;
}

