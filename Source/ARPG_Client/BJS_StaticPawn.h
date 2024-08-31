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
	// Sets default values for this pawn's properties
	ABJS_StaticPawn();

	virtual void SetSkeletalMesh(class USkeletalMesh* Mesh);
	virtual void SetStaticMesh(class UStaticMesh* Mesh);

	virtual void SetVisible(bool Flag);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void CreateDefaultMesh(FString MeshPath);
	void CreateSKeletalDefaultMesh(FString MeshPath);
	void CreateCamerCompoent();

	class USpringArmComponent* GetSpringArmComponent();
	class UCameraComponent* GetFollowCamera();
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* DefaultMeshComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* DefaultSkeletalMeshComponent;
	
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
};
