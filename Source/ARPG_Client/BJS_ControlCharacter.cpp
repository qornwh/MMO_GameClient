// Fill out your copyright notice in the Description page of Project Settings.


#include "BJS_ControlCharacter.h"

#include "BJS_Bullet.h"
#include "BJS_CharaterState.h"
#include "BJS_GameInstance.h"
#include "BJS_GameUI.h"
#include "BJS_InGameMode.h"
#include "BJS_SocketActor.h"
#include "BulletCollisionUtils.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameClient.pb.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "SkillStruct.h"
#include "Components/WidgetComponent.h"

ABJS_ControlCharacter::ABJS_ControlCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 0.2f;
	GetCapsuleComponent()->InitCapsuleSize(30.f, 96.0f);

	// 일단 yaw(수직)방향만 
	// GetCharacter()->bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	// GetCharacter()->bUseControllerRotationRoll = false;

	// 여기서 점프 높이, 중력, 스피드 설정
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->GravityScale = 1.75f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;

	// 스프링암 설정
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom")); // SpringArmComponent 컴포넌트 생성
	CameraBoom->SetupAttachment(RootComponent); // 루트 아래에 추가
	CameraBoom->TargetArmLength = 400.0f; // 카메라의 거리	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// 카메리 설정
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false; // arm 방향 따라감

	// 저격 카메라 설정
	FpsCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FpsCamera"));
	FpsCamera->SetupAttachment(GetMesh());
	FpsCamera->bUsePawnControlRotation = true;

	// 게임 ui 인터페이스
	FollowCamera->SetActive(true);
	FpsCamera->SetActive(false);
	SetInputAction();
	HpBar->DestroyComponent();

	// 불렛 초기화
	PresentAttackNumber = 0;
}

void ABJS_ControlCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void ABJS_ControlCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* _PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(_PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	FpsCamera->SetRelativeLocation(FpsCameraLocation);
	FpsCamera->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));
	
	auto instance = Cast<UBJS_GameInstance>(GetGameInstance());
	instance->GetMyState()->SetTarget(this);
	SetState(instance->GetMyState());
	IsHitted = true;
	
	LoadInfo(instance->GetCurCharaterType(), instance->GetCurWeaponType());

	// 현재 GameUi들고온다
	auto mode = Cast<ABJS_GameModeBase>(GetWorld()->GetAuthGameMode());
	auto mainUi = Cast<UBJS_GameUI>(mode->GetCurrentWidget());

	// 쿨타임ui 업데이트
	OnSkillIconUI.BindUObject(mainUi, &UBJS_GameUI::CoolTimeUpdate);
}

void ABJS_ControlCharacter::Tick(float DeltaTime)
{
	SendMoveMessage();
	UpdateBuff(DeltaTime);
	SetAttackTimer(DeltaTime);
	for (auto& entry : SkillList)
	{
		int32 keyBind = entry.Key;
		int32 skillCode = entry.Value;

		if (BuffList.Contains(skillCode))
		{
			if (BuffList[skillCode].Code == skillCode)
			{
				float coolTime = BuffList[skillCode].CoolDownRemaining;
				OnSkillIconUI.Execute(keyBind, coolTime);
			}
		}
	}
	// 여기서 총알 충돌처리한다.
	BulletAttackObject();
}

void ABJS_ControlCharacter::Move(float DeltaTime)
{
	// not use 
}

void ABJS_ControlCharacter::Move(const FInputActionValue& Value)
{
	if (!IsAttack)
		return;

	// 방향 입력값
	FVector2D MovementVector = Value.Get<FVector2D>();

	// 컨트롤러 정면방향
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// 바라보는 방향으로 이동
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	GetCharacterMovement()->MaxWalkSpeed = State->GetSpeed();
	SetActorRotation(YawRotation);
	AddMovementInput(ForwardDirection, MovementVector.Y);
	AddMovementInput(RightDirection, MovementVector.X);
}

void ABJS_ControlCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	if (IsAim)
	{
		// 사격 모드이면 회전 하지 않도록 한다. (에임 x)
		// AddControllerPitchInput(LookAxisVector.Y);
		//
		// FRotator ControlRotation = GetControlRotation();
		// UE_LOG(LogTemp, Log, TEXT("ControlRotation.Pitch %f !!!"), ControlRotation.Pitch);
		//
		// if (ControlRotation.Pitch > 180)
		// 	ControlRotation.Pitch -= 360.f;
		//
		// if (ControlRotation.Pitch > 30 && ControlRotation.Pitch < 180)
		// 	ControlRotation.Pitch = 30.f;
		// else if (ControlRotation.Pitch < 0 && ControlRotation.Pitch < -30)
		// 	ControlRotation.Pitch = -30.f;
		//
		// FpsCamera->SetRelativeLocation(FVector(-40.f, -40.f, 165.f - ControlRotation.Pitch));
		// GetController()->SetControlRotation(ControlRotation);
	}
	else
	{
		const FRotator ActorRotate = GetActorRotation();
		const FRotator Rotation = GetControlRotation();
		const FRotator YawRotation(ActorRotate.Pitch, Rotation.Yaw, ActorRotate.Roll);

		// 바라보는 방향으로 이동
		SetActorRotation(YawRotation);
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ABJS_ControlCharacter::ChangeCamera()
{
	if (FpsCamera->IsActive())
	{
		IsAim = false;
		FpsCamera->SetActive(false);
		FollowCamera->SetActive(true);
	}
	else
	{
		IsAim = true;
		FpsCamera->SetActive(true);
		FollowCamera->SetActive(false);
		// 일단 총든 모드에서는 컨트롤 방향 고정
		FRotator ControlRotation(0.f, GetControlRotation().Yaw, 0.f);
		GetController()->SetControlRotation(ControlRotation);
	}
	SendAimMessage();
	ChangeWeaponSocket();
}

void ABJS_ControlCharacter::SetInputAction()
{
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> DEFAULT_CONTEXT
		(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/MyGame/Input/IMC_Default.IMC_Default'"));
	if (DEFAULT_CONTEXT.Succeeded())
		DefaultMappingContext = DEFAULT_CONTEXT.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> IA_Jump
		(TEXT("/Script/EnhancedInput.InputAction'/Game/MyGame/Input/Actions/IA_Jump.IA_Jump'"));
	if (IA_Jump.Succeeded())
		JumpAction = IA_Jump.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> IA_Move
		(TEXT("/Script/EnhancedInput.InputAction'/Game/MyGame/Input/Actions/IA_Move.IA_Move'"));
	if (IA_Move.Succeeded())
		MoveAction = IA_Move.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> IA_Look
		(TEXT("/Script/EnhancedInput.InputAction'/Game/MyGame/Input/Actions/IA_Look.IA_Look'"));
	if (IA_Look.Succeeded())
		LookAction = IA_Look.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> IA_Right
		(TEXT("/Script/EnhancedInput.InputAction'/Game/MyGame/Input/Actions/IA_RightClick.IA_RightClick'"));
	if (IA_Right.Succeeded())
		RightClickAction = IA_Right.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> IA_Left
		(TEXT("/Script/EnhancedInput.InputAction'/Game/MyGame/Input/Actions/IA_LeftClick.IA_LeftClick'"));
	if (IA_Right.Succeeded())
		LeftClickAction = IA_Left.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> IA_Skill
		(TEXT("/Script/EnhancedInput.InputAction'/Game/MyGame/Input/Actions/IA_Skill.IA_Skill'"));
	if (IA_Skill.Succeeded())
		SkillClickAction = IA_Skill.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> IA_Inventory
		(TEXT("/Script/EnhancedInput.InputAction'/Game/MyGame/Input/Actions/IA_Inventory.IA_Inventory'"));
	if (IA_Inventory.Succeeded())
		InventoryAction = IA_Inventory.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> IA_O
		(TEXT("/Script/EnhancedInput.InputAction'/Game/MyGame/Input/Actions/IA_O.IA_O'"));
	if (IA_O.Succeeded())
		KeyOAction = IA_O.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> IA_Ctrl
		(TEXT("/Script/EnhancedInput.InputAction'/Game/MyGame/Input/Actions/IA_Ctrl.IA_Ctrl'"));
	if (IA_Ctrl.Succeeded())
		KeyCtrlAction = IA_Ctrl.Object;
}

void ABJS_ControlCharacter::SendAttackMessage(int32 SkillCode)
{
	auto socketActor = Cast<ABJS_GameModeBase>(GetWorld()->GetAuthGameMode())->GetSocketActor();
	if (socketActor)
	{
		protocol::CAttack pkt;
		FVector CurPos = GetActorLocation();
		FRotator CurRot = GetActorRotation();
		pkt.set_skillcode(SkillCode);
		pkt.set_attacknumber(PresentAttackNumber);
		pkt.set_uuid(State->GetUUid());
		protocol::Position* position = new protocol::Position();
		position->set_x(CurPos.X);
		position->set_y(CurPos.Y);
		position->set_z(CurPos.Z);
		position->set_yaw(CurRot.Yaw);
		pkt.set_allocated_position(position);
		socketActor->SendMessage(pkt, protocol::MessageCode::C_ATTACK);
	}
}

void ABJS_ControlCharacter::SendAimMessage()
{
	auto socketActor = Cast<ABJS_GameModeBase>(GetWorld()->GetAuthGameMode())->GetSocketActor();
	if (socketActor)
	{
		protocol::CPlayerAim pkt;
		pkt.set_isaim(IsAim);
		pkt.set_uuid(State->GetUUid());
		
		socketActor->SendMessage(pkt, protocol::MessageCode::C_PLAYERAIM);
	}
}

void ABJS_ControlCharacter::SendJumpMessage(bool flag)
{
	auto socketActor = Cast<ABJS_GameModeBase>(GetWorld()->GetAuthGameMode())->GetSocketActor();
	if (socketActor)
	{
		protocol::CPlayerJump pkt;
		pkt.set_isjump(flag);
		pkt.set_uuid(State->GetUUid());
		
		socketActor->SendMessage(pkt, protocol::MessageCode::C_PLAYERJUMP);
	}
}

void ABJS_ControlCharacter::SendMoveMessage()
{
	FVector CurPos = GetActorLocation();
	FRotator CurRot = GetActorRotation();
	if (!CurPos.Equals(PrePosition) || CurRot.Yaw != PreRotation.Yaw)
	{
		auto socketActor = Cast<ABJS_GameModeBase>(GetWorld()->GetAuthGameMode())->GetSocketActor();
		if (socketActor)
		{
			protocol::SMove pkt;
			pkt.set_is_monster(false);
			protocol::Position* position = new protocol::Position();
			position->set_x(CurPos.X);
			position->set_y(CurPos.Y);
			position->set_z(CurPos.Z);
			position->set_yaw(CurRot.Yaw);
			pkt.set_allocated_position(position);
			socketActor->SendMessage(pkt, protocol::MessageCode::S_MOVE);
		}
		PrePosition = CurPos;
		PreRotation = CurRot;
	}
}

void ABJS_ControlCharacter::SendAttackObjectMessage(int32 SkillCode, int32 AttackNumber, const TArray<TTuple<int, ABJS_Character*>>& successAttacks)
{
	FVector CurPos = GetActorLocation();
	FRotator CurRot = GetActorRotation();
	auto socketActor = Cast<ABJS_GameModeBase>(GetWorld()->GetAuthGameMode())->GetSocketActor();
	if (socketActor)
	{
		protocol::UserAttack pkt;
		pkt.set_skillcode(SkillCode);
		pkt.set_attacknumber(AttackNumber);
		protocol::Position* position = new protocol::Position();
		position->set_x(CurPos.X);
		position->set_y(CurPos.Y);
		position->set_z(CurPos.Z);
		position->set_yaw(CurRot.Yaw);
		pkt.set_allocated_position(position);

		for (int i = 0; i < successAttacks.Num(); i++)
		{
			auto success = successAttacks[i];
			pkt.add_targetcodes(success.Value->GetState()->GetUUid());
		}
		socketActor->SendMessage(pkt, protocol::MessageCode::C_ATTACKS);
	}
}

void ABJS_ControlCharacter::BulletAttackObject()
{
	auto mode = Cast<ABJS_InGameMode>(GetWorld()->GetAuthGameMode());
	if (mode == nullptr)
		return;
			
	for (auto e : Bullets)
	{
		ABJS_Bullet* bullet = e.Value;
		if (bullet)
		{
			// 공격 판단.
			// 서버 현재 포지션, 타겟, 공격 코드 전달.
			float bulletRadius = bullet->GetScaledCapsuleRadius();
			float bulletWidth = bullet->GetCollisionWidth();
			int32 skillCode = bullet->GetSkillCode();
			int32 attackNumber = e.Key;
			FVector presentPos = bullet->GetPresentPos();
			FVector position = bullet->GetActorLocation();

			if (bulletWidth > 0.f)
			{
				FVector dVector = position.GetSafeNormal() * bulletWidth;
				presentPos = position - dVector;			
				presentPos = position + dVector;					
			}

			DrawDebugCapsule(GetWorld(), presentPos, bulletRadius, bulletRadius, bullet->GetActorRotation().Quaternion(), FColor::Yellow, true, -1, 0, 1);
			DrawDebugCapsule(GetWorld(), position, bulletRadius, bulletRadius, bullet->GetActorRotation().Quaternion(), FColor::Yellow, true, -1, 0, 1);
			
			TArray<TTuple<int, ABJS_Character*>> successAttacks;
			for (auto& monsterEntry : mode->GetMonsterStateList())
			{
				TSharedPtr<BJS_CharaterState> monsterState = monsterEntry.Value;
				if (monsterState == nullptr)
					continue;
				
				ABJS_Character* monster = monsterState->GetTarget();
				FVector monsterPosition = monster->GetActorLocation();
				auto monsterCollision = monster->GetCapsuleComponent();
				float monsterRadius = monsterCollision->GetScaledCapsuleRadius();
				float monsterHight = monsterCollision->GetScaledCapsuleHalfHeight();

				if (BulletCollisionUtils::CheckAABB2D(position, presentPos, monsterPosition, bulletRadius - monsterRadius))
				{
					float d = BulletCollisionUtils::CapsuleToCircleDistance2D(position, presentPos, monsterPosition);

					if (d <= monsterRadius + bulletRadius)
					{
						successAttacks.Add({d, monster});
					}
				}
			
				// AABB 비교
				// if (FMath::Min(position.X, presentPos.X) - bulletRadius <= monsterPosition.X && monsterPosition.X <=  FMath::Max(position.X, presentPos.X) + bulletRadius)
				// {
				// 	if (FMath::Min(position.Y, presentPos.Y) - bulletRadius <= monsterPosition.Y && monsterPosition.Y <=  FMath::Max(position.Y, presentPos.Y) + bulletRadius)
				// 	{
				// 		선분(PresentPos-position)과 점(캐릭터는 전부 캡슐로 위에서보므로 원이된다)거리
				// 		FVector bulletToPos = presentPos - position;
				// 		FVector monsterToPos = monsterPosition - position;
				// 		DrawDebugCapsule(GetWorld(), monster->GetActorLocation(), monsterHight, monsterRadius, monster->GetActorRotation().Quaternion(), FColor::Blue, true, -1, 0, 1);
				// 		
				// 		float dot = FVector::DotProduct(monsterToPos, bulletToPos);
				// 		float let2sqrt = bulletToPos.SquaredLength();
				// 		float t = dot / let2sqrt;
				// 		float d;
				// 		
				// 		if (t < 0.f)
				// 		{
				// 			d = FVector(monsterPosition - position).Length();
				// 		}
				// 		else if (t > 1.f)
				// 		{
				// 			d = FVector(monsterPosition - presentPos).Length();
				// 		}
				// 		else
				// 		{
				// 			FVector h(position.X + t * bulletToPos.X, position.Y + t * bulletToPos.Y, position.Z);
				// 			d = (monsterPosition - h).Length();
				// 		}
				// 		
				// 		if (d <= monsterRadius + bulletRadius)
				// 		{
				// 			// 충돌 성공
				// 			Bullets[attackNumber] = nullptr;
				// 			Bullets.Remove(AttackNumber);
				// 		}
				// 	}
				// }
			}
			
			bullet->SetPresentPos();
			if (Bullets[attackNumber] != nullptr && bullet->IsCoolDown())
			{
				Bullets[attackNumber] = nullptr;
				Bullets.Remove(PresentAttackNumber);
			}
			
			successAttacks.Sort([](TTuple<int, ABJS_Character*> a, TTuple<int, ABJS_Character*> b){
				return a.Key < b.Key;	
			});

			if (successAttacks.Num() > 0)
			{
				while (successAttacks.Num() > 1)
				{
					successAttacks.Pop();
				}
				
				SendAttackObjectMessage(skillCode, attackNumber, successAttacks);
			}
		}
	}
}

void ABJS_ControlCharacter::SetState(TSharedPtr<BJS_CharaterState> state)
{
	check(state.IsValid());
	State = state;
}

void ABJS_ControlCharacter::PlayAttack(int32 Code, bool ignore)
{
	Super::PlayAttack(Code, ignore);
	++PresentAttackNumber;
	PresentAttackNumber %= BulletNumberMax;
}

void ABJS_ControlCharacter::PlaySkill(int32 Code, bool ignore)
{
	Super::PlaySkill(Code, ignore);
}

void ABJS_ControlCharacter::ShowInventoryUI()
{
	auto mode = Cast<ABJS_InGameMode>(GetWorld()->GetAuthGameMode());
	if (mode)
	{
		mode->ChangeInventoryUI();
	}
}

void ABJS_ControlCharacter::ShowMailBoxUI()
{
	auto mode = Cast<ABJS_InGameMode>(GetWorld()->GetAuthGameMode());
	if (mode)
	{
		mode->ChangeMailBoxUI();
	}
}

void ABJS_ControlCharacter::OpenFriendUI()
{
	auto mode = Cast<ABJS_InGameMode>(GetWorld()->GetAuthGameMode());
	if (mode)
	{
		mode->OpenFriendUI();
	}
}

void ABJS_ControlCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// 인풋 바인딩 설정
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//Jumping
		// EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ABJS_ControlCharacter::StartJump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ABJS_ControlCharacter::StartJump);
		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABJS_ControlCharacter::Move);
		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ABJS_ControlCharacter::Look);
		//rightClick
		EnhancedInputComponent->BindAction(RightClickAction, ETriggerEvent::Completed, this, &ABJS_ControlCharacter::ChangeCamera);
		//leftClick
		EnhancedInputComponent->BindAction(LeftClickAction, ETriggerEvent::Started, this, &ABJS_ControlCharacter::Attack);
		//SkillClick
		EnhancedInputComponent->BindAction(SkillClickAction, ETriggerEvent::Started, this, &ABJS_ControlCharacter::Skill);
		//Ivnentory
		EnhancedInputComponent->BindAction(InventoryAction, ETriggerEvent::Completed, this, &ABJS_ControlCharacter::ShowInventoryUI);
		//Friend
		EnhancedInputComponent->BindAction(KeyOAction, ETriggerEvent::Completed, this, &ABJS_ControlCharacter::OpenFriendUI);
		//OnMouse
		EnhancedInputComponent->BindAction(KeyCtrlAction, ETriggerEvent::Completed, this, &ABJS_ControlCharacter::ViewMouse);
	}
}

void ABJS_ControlCharacter::StopJump()
{
	Super::StopJump();
	SendJumpMessage(false);
}

void ABJS_ControlCharacter::StartJump()
{
	SendJumpMessage(true);
	Super::StartJump();
}

void ABJS_ControlCharacter::Skill(const FInputActionValue& Value)
{
	// 일단 인풋액션 설정으로 둠
	auto result = Value.Get<FVector>();
	int32 InputCode = result.X;
	if (!SkillList.Contains(InputCode))
		return;
	int32 SkillCode = SkillList[InputCode];
	SendAttackMessage(SkillCode);

	if (BuffList[SkillCode].CoolDownRemaining == 0.f)
	{
		auto instance = Cast<UBJS_GameInstance>(GetWorld()->GetGameInstance());
		if (instance)
		{
			auto skill = instance->GetSkillStructs()[SkillCode];
			auto mode = Cast<ABJS_InGameMode>(GetWorld()->GetAuthGameMode());
			if (skill->Type == CharaterSkill::SKILLTYPES::HP && mode)
			{
				mode->TakeHeal(SkillCode, State);
			}
			else if (skill->Type == CharaterSkill::SKILLTYPES::ATT && !IsAim)
			{
				return;
			}
		}
		PlaySkill(SkillCode);
	}
}

void ABJS_ControlCharacter::ViewMouse(const FInputActionValue& Value)
{
	auto mode = Cast<ABJS_InGameMode>(GetWorld()->GetAuthGameMode());
	if (mode)
	{
		ShowMouseCursor = !ShowMouseCursor;
		mode->SetShowMouseCousor(ShowMouseCursor);
	}
}

void ABJS_ControlCharacter::Attack()
{
	SendAttackMessage();
	PlayAttack(0);
}
