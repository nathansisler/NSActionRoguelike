// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NSInteractionComponent.h"
#include "Animation/AnimMontage.h"
#include "NSCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UNSInteractionComponent;
class UAnimMontage;

enum MyEnum
{
	MagicProjectileE,
	BlackholeProjectileE,
	TeleportProjectileE
};

UCLASS()
class NSACTIONROGUELIKE_API ANSCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANSCharacter();

protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> MagicProjectile;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> BlackholeProjectile;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> TeleportProjectile;

	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* AttackAnim;

	FTimerHandle TimerHandle_PrimaryAttack;

protected:

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere)
	UNSInteractionComponent* InteractionComp;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float value);

	void MoveRight(float value);

	void SetAttackTimer(const MyEnum e);

	void Attack_TimeElapsed(TSubclassOf<AActor> Projectile);

	void Teleport(AActor* Projectile);

	void PrimaryInteract();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
