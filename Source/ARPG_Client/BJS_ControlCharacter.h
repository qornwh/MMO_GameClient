// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BJS_Character.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "BJS_ControlCharacter.generated.h"

DECLARE_DELEGATE(FOnAttackAcess)
DECLARE_DELEGATE_TwoParams(FOnSkillIconUI, int32, float)

/**
 * 
 */
UCLASS()
class ARPG_CLIENT_API ABJS_ControlCharacter : public ABJS_Character
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FpsCamera;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* RightClickAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LeftClickAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* SkillClickAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* InventoryAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* KeyOAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* KeyCtrlAction;

	bool ShowMouseCursor = false;

public:
	ABJS_ControlCharacter();

	FOnSkillIconUI OnSkillIconUI;

protected:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void Move(float DeltaTime) override;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Skill(const FInputActionValue& Value);
	void ViewMouse(const FInputActionValue& Value);
	virtual void Attack() override;
	virtual void StopJump() override;
	virtual void StartJump() override;
	void ChangeCamera();
	void SetInputAction();
	void SendAttackMessage(int32 Code = 0);
	void SendAimMessage();
	void SendJumpMessage(bool flag);
	void SendMoveMessage();
	virtual void SetState(TSharedPtr<BJS_CharaterState> state) override;

public:
	void ShowInventoryUI();
	void ShowMailBoxUI();
	void OpenFriendUI();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	float time = 0;
	FVector FpsCameraLocation{-40.f, -140.f, 165.f};
	FVector PrePosition{0.f};
	FRotator PreRotation{0.f};
};
