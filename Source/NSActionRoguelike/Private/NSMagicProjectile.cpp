// Fill out your copyright notice in the Description page of Project Settings.


#include "NSMagicProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ANSMagicProjectile::ANSMagicProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


}

// Called when the game starts or when spawned
void ANSMagicProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ANSMagicProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

