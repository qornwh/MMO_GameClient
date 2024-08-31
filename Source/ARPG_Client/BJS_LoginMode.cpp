// Fill out your copyright notice in the Description page of Project Settings.


#include "BJS_LoginMode.h"

#include "BJS_GameInstance.h"
#include "BJS_LoginPawn.h"
#include "BJS_LoginWidget.h"
#include "BJS_MainWidget.h"
#include "BJS_PromptWidget.h"
#include "BJS_SelectWidget.h"
#include "BJS_SocketActor.h"
#include "BJS_StoreWidget.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

void ABJS_LoginMode::LoadSubClass()
{
	Super::LoadSubClass();

	static ConstructorHelpers::FClassFinder<UUserWidget> UI_LOGIN_HUD(
		TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/MyGame/UMG/BJS_WBP_Login.BJS_WBP_Login_C'"));
	widgetMap.Add(EWidgetEnum::E_LOGIN, UI_LOGIN_HUD.Class);
	check(widgetMap[EWidgetEnum::E_LOGIN] != nullptr);

	static ConstructorHelpers::FClassFinder<UUserWidget> UI_MAIN_HUD(
		TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/MyGame/UMG/BJS_WBP_Main.BJS_WBP_Main_C'"));
	widgetMap.Add(EWidgetEnum::E_MAIN, UI_MAIN_HUD.Class);
	check(widgetMap[EWidgetEnum::E_MAIN] != nullptr);

	static ConstructorHelpers::FClassFinder<UUserWidget> UI_SELECT_HUD(
		TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/MyGame/UMG/BJS_WBP_Select.BJS_WBP_Select_C'"));
	widgetMap.Add(EWidgetEnum::E_SELECT, UI_SELECT_HUD.Class);
	check(widgetMap[EWidgetEnum::E_SELECT] != nullptr);

	static ConstructorHelpers::FClassFinder<UUserWidget> UI_STORE_HUD(
		TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/MyGame/UMG/BJS_WBP_Store.BJS_WBP_Store_C'"));
	widgetMap.Add(EWidgetEnum::E_CSTORE, UI_STORE_HUD.Class);
	check(widgetMap[EWidgetEnum::E_CSTORE] != nullptr);

	widgetMap.Add(EWidgetEnum::E_WSTORE, UI_STORE_HUD.Class);
	check(widgetMap[EWidgetEnum::E_WSTORE] != nullptr);

	// 이미지 셋팅
	static ConstructorHelpers::FObjectFinder<UTexture2D> CHARATER_IMAGE_1(TEXT("/Script/Engine.Texture2D'/Game/MyGame/Assets/Image/CharaterPng1.CharaterPng1'"));
	CharaterImageList.Add(CHARATER_IMAGE_1.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> CHARATER_IMAGE_2(TEXT("/Script/Engine.Texture2D'/Game/MyGame/Assets/Image/CharaterPng2.CharaterPng2'"));
	CharaterImageList.Add(CHARATER_IMAGE_2.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> CHARATER_IMAGE_3(TEXT("/Script/Engine.Texture2D'/Game/MyGame/Assets/Image/CharaterPng3.CharaterPng3'"));
	CharaterImageList.Add(CHARATER_IMAGE_2.Object);
	check(CharaterImageList.Num() == 3);
	
	static ConstructorHelpers::FObjectFinder<UTexture2D> WEAPON_IMAGE_1(TEXT("/Script/Engine.Texture2D'/Game/MyGame/Assets/Image/WeaponPng1.WeaponPng1'"));
	WeaponImageList.Add(WEAPON_IMAGE_1.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> WEAPON_IMAGE_2(TEXT("/Script/Engine.Texture2D'/Game/MyGame/Assets/Image/WeaponPng2.WeaponPng2'"));
	WeaponImageList.Add(WEAPON_IMAGE_2.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> WEAPON_IMAGE_3(TEXT("/Script/Engine.Texture2D'/Game/MyGame/Assets/Image/WeaponPng3.WeaponPng3'"));
	WeaponImageList.Add(WEAPON_IMAGE_3.Object);
	check(WeaponImageList.Num() == 3);
}

void ABJS_LoginMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
}

void ABJS_LoginMode::BeginPlay()
{
	Super::BeginPlay();

	FVector SpawnLocation = FVector(0.f, 0.f, 0.f);
	FRotator SpawnRotation = FRotator(0.f, 0.f, 0.f);
	SocketActor = GetWorld()->SpawnActor<ABJS_SocketActor>(ABJS_SocketActor::StaticClass(), SpawnLocation,
	                                                       SpawnRotation);
	SetShowMouseCousor(true);
}

ABJS_LoginMode::ABJS_LoginMode()
{
	static ConstructorHelpers::FClassFinder<AActor> defaultPawnClass(
		TEXT("/Script/Engine.Blueprint'/Game/MyGame/BluePrint/BJS_BP_LoginPawn.BJS_BP_LoginPawn_C'"));
	DefaultPawnClass = defaultPawnClass.Class;
	check(DefaultPawnClass != nullptr);

	// 일단 생성자에서 가상함수 바인딩은 안된다. C++ 정말 애매한 문제이다.
	ABJS_LoginMode::LoadSubClass();

	CurrentWidget = CreateWidget<UBJS_UserWidgetBase>(GetWorld(), widgetMap[EWidgetEnum::E_LOGIN]);
	if (CurrentWidget)
	{
		CurrentWidget->AddToViewport();
		auto LoginWidget_BJS = Cast<UBJS_LoginWidget>(CurrentWidget);
		if (LoginWidget_BJS)
			LoginWidget_BJS->OnLoginSuccess.BindUObject(this, &ABJS_LoginMode::SetMainWidget);
	}

	CharaterMeshList.Reserve(3);
	WeaponMeshList.Reserve(3);

	// 캐릭터 설정
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> LoginCharaterMesh(
		TEXT("/Script/Engine.SkeletalMesh'/Game/MyGame/Assets/Charater/LoginMesh/LoginMesh.LoginMesh'"));
	if (LoginCharaterMesh.Succeeded())
	{
		CharaterMeshList.Add(LoginCharaterMesh.Object);
	}

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> LoginCharaterMesh2(
		TEXT("/Script/Engine.SkeletalMesh'/Game/MyGame/Assets/Charater/LoginMesh/LoginMesh_2.LoginMesh_2'"));
	if (LoginCharaterMesh2.Succeeded())
	{
		CharaterMeshList.Add(LoginCharaterMesh2.Object);
	}

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> LoginCharaterMesh3(
		TEXT("/Script/Engine.SkeletalMesh'/Game/MyGame/Assets/Charater/LoginMesh/LoginMesh_3.LoginMesh_3'"));
	if (LoginCharaterMesh3.Succeeded())
	{
		CharaterMeshList.Add(LoginCharaterMesh3.Object);
	}
	
	// 무기설정
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> LoginWeaponMesh(
		TEXT("/Script/Engine.SkeletalMesh'/Game/MyGame/Assets/Weapon/WeaponsMesh/Grenade_Launcher_A.Grenade_Launcher_A'"));
	if (LoginWeaponMesh.Succeeded())
	{
		WeaponMeshList.Add(LoginWeaponMesh.Object);
	}
	
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> LoginWeaponMesh2(
		TEXT("/Script/Engine.SkeletalMesh'/Game/MyGame/Assets/Weapon/WeaponsMesh/Rocket_Launcher_A.Rocket_Launcher_A'"));
	if (LoginWeaponMesh2.Succeeded())
	{
		WeaponMeshList.Add(LoginWeaponMesh2.Object);
	}
	
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> LoginWeaponMesh3(
		TEXT("/Script/Engine.SkeletalMesh'/Game/MyGame/Assets/Weapon/WeaponsMesh/Shotgun_A.Shotgun_A'"));
	if (LoginWeaponMesh3.Succeeded())
	{
		WeaponMeshList.Add(LoginWeaponMesh3.Object);
	}
}

void ABJS_LoginMode::SetMainWidget()
{
	SetShowMouseCousor(true);
	SwitchWidget(widgetMap[EWidgetEnum::E_MAIN]);

	auto Widget = Cast<UBJS_MainWidget>(CurrentWidget);
	if (Widget)
	{
		Widget->OnSelect.BindUObject(this, &ABJS_LoginMode::SetSelectWidget);
		Widget->OnCStore.BindUObject(this, &ABJS_LoginMode::SetCharaterStoerWidget);
		Widget->OnWStore.BindUObject(this, &ABJS_LoginMode::SetWeaponStoerWidget);
		Widget->OnStart.BindUObject(this, &ABJS_LoginMode::StartGame);
		Widget->BJS_UpdateWidget();
	}
	
	if (UGameplayStatics::GetPlayerPawn(GetWorld(), 0))
	{
		auto player = Cast<ABJS_LoginPawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
		auto instance = Cast<UBJS_GameInstance>(GetGameInstance());

		if (player && instance)
		{
			if (instance->GetCurCharaterType() == 0)
				player->SetSkeletalMesh(nullptr);
			else
			{
				player->SetSkeletalMesh(CharaterMeshList[instance->GetCurCharaterType() - 1]);
				if (instance->GetCurWeaponType() <= 0)
					player->CreateWeaponActor(nullptr);
				else
					player->CreateWeaponActor(WeaponMeshList[instance->GetCurWeaponType() - 1]);
			}
			player->SetVisible(true);
		}
	}
}

void ABJS_LoginMode::SetSelectWidget()
{
	SwitchWidget(widgetMap[EWidgetEnum::E_SELECT]);

	auto Widget = Cast<UBJS_SelectWidget>(CurrentWidget);
	if (Widget)
	{
		Widget->OnBack.BindUObject(this, &ABJS_LoginMode::SetMainWidget);
		Widget->BJS_UpdateWidget();
		Widget->SetSelectImage(CharaterImageList, WeaponImageList);
	}
}

void ABJS_LoginMode::SetCharaterStoerWidget()
{
	SwitchWidget(widgetMap[EWidgetEnum::E_CSTORE]);

	auto Widget = Cast<UBJS_StoreWidget>(CurrentWidget);
	if (Widget)
	{
		Widget->SetMode(0);
		Widget->OnBack.BindUObject(this, &ABJS_LoginMode::SetMainWidget);
		Widget->BJS_UpdateWidget();
		Widget->SetStoreImage(CharaterImageList);
	}
}

void ABJS_LoginMode::SetWeaponStoerWidget()
{
	SwitchWidget(widgetMap[EWidgetEnum::E_WSTORE]);

	auto Widget = Cast<UBJS_StoreWidget>(CurrentWidget);
	if (Widget)
	{
		Widget->SetMode(1);
		Widget->OnBack.BindUObject(this, &ABJS_LoginMode::SetMainWidget);
		Widget->BJS_UpdateWidget();
		Widget->SetStoreImage(WeaponImageList);
	}
}

void ABJS_LoginMode::StartGame()
{
	auto instance = Cast<UBJS_GameInstance>(GetGameInstance());
	if (instance)
	{
		int characterType = instance->GetCurCharaterType();
		int weaponType = instance->GetCurWeaponType();
		if (characterType > 0 && weaponType > 0)
		{
			UGameplayStatics::OpenLevel(this, TEXT("GameMap"));
		}
		else
		{
			auto PromptWidget = CreateWidget<UUserWidget>(GetWorld(), instance->GetPrompt());
			if (PromptWidget)
			{
				PromptWidget->AddToViewport();
				auto widget = Cast<UBJS_PromptWidget>(PromptWidget);
				if (widget)
				{
					FString str = FString(TEXT("현재 캐릭터와 무기가 설정되지 않았습니다."));
					widget->SetText(str);
				}
			}
		}
	}
}

void ABJS_LoginMode::SwitchWidget(TSubclassOf<UUserWidget> ChangeWidgetClass)
{
	auto instance = Cast<UBJS_GameInstance>(GetGameInstance());
	
	if (UGameplayStatics::GetPlayerPawn(GetWorld(), 0))
	{
		auto player = Cast<ABJS_LoginPawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
		if (player)
		{
			player->SetVisible(false);
		}
	}

	if (CurrentWidget)
	{
		CurrentWidget->RemoveFromParent();
		CurrentWidget = nullptr;
		if (instance)
			SetCurrentWidget(nullptr);
	}

	if (ChangeWidgetClass)
	{
		CurrentWidget = CreateWidget<UBJS_UserWidgetBase>(GetWorld(), ChangeWidgetClass);
		if (CurrentWidget)
		{
			CurrentWidget->AddToViewport();
			if (instance)
				SetCurrentWidget(CurrentWidget);
		}
	}
}
