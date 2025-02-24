// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BJS_StaticPawn.generated.h"

UCLASS()
class ARPG_CLIENT_API ABJS_StaticPawn : public APawn
{
	GENERATED_BODY()

public:
	ABJS_StaticPawn();

	virtual void SetSkeletalMesh(class USkeletalMesh* Mesh);
	virtual void SetStaticMesh(class UStaticMesh* Mesh);
	virtual void SetVisible(bool Flag);

protected:
	virtual void BeginPlay() override;
	void CreateCamerCompoent();

	class USpringArmComponent* GetSpringArmComponent();
	class UCameraComponent* GetFollowCamera();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* DefaultMeshComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* DefaultSkeletalMeshComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
};
