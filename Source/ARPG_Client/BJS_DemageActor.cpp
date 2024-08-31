// Fill out your copyright notice in the Description page of Project Settings.


#include "BJS_DemageActor.h"

#include <string>

#include "BJS_DemageWidget.h"
#include "Components/WidgetComponent.h"

// Sets default values
ABJS_DemageActor::ABJS_DemageActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = RootSceneComponent;
	
	DemageWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("DemageWidget"));
	DemageWidget->SetupAttachment(RootComponent);
	DemageWidget->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	DemageWidget->SetWidgetSpace(EWidgetSpace::World);
	
	static ConstructorHelpers::FClassFinder<UUserWidget> DamageWidget(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/MyGame/UMG/BJS_WBP_Demage.BJS_WBP_Demage_C'"));
	if (DamageWidget.Succeeded())
	{
		DemageWidget->SetWidgetClass(DamageWidget.Class);
		DemageWidget->SetDrawSize(FVector2D(150.f, 30.f));
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("DemageWidget !!!!"));
	}

	PrimaryActorTick.TickInterval = TickTime;
}

// Called when the game starts or when spawned
void ABJS_DemageActor::BeginPlay()
{
	Super::BeginPlay();
	SetRandomLoc();
}

// Called every frame
void ABJS_DemageActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (OnOff)
		Destroy();
	OnOff = true;
}

void ABJS_DemageActor::SetDemage(int32 Demage)
{
	if (DemageWidget)
	{
		auto Widget = Cast<UBJS_DemageWidget>(DemageWidget->GetUserWidgetObject());
		if (Widget)
		{
			Widget->SetDemage(FString::FromInt(Demage));
		}
	}
}

void ABJS_DemageActor::SetHeal(int32 Heal)
{
	if (DemageWidget)
	{
		auto Widget = Cast<UBJS_DemageWidget>(DemageWidget->GetUserWidgetObject());
		if (Widget)
		{
			Widget->SetHeal(FString::FromInt(Heal));
		}
	}
}

void ABJS_DemageActor::SetRandomLoc()
{
	FVector RightVector = GetActorRightVector();
	FVector CurrentVector = GetActorLocation();
	int32 rightVal = FMath::RandRange(-20, 20);
	CurrentVector.Z += FMath::RandRange(-30, 30);
	CurrentVector.Z += 100.f;
			
	FVector NewLocation = CurrentVector + (RightVector * rightVal);
	SetActorLocation(NewLocation);
}

