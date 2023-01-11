// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NSProjectile.h"
#include "NSMagicProjectile.generated.h"

//class USphereComponent;
//class UProjectileMovementComponent;
//class UParticleSystemComponent;

UCLASS()
class NSACTIONROGUELIKE_API ANSMagicProjectile : public ANSProjectile
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANSMagicProjectile();

protected:

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	//USphereComponent* SphereComp;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	//UProjectileMovementComponent* MovementComp;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	//UParticleSystemComponent* EffectComp;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
