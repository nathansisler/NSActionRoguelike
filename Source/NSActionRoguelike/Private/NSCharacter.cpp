// Fill out your copyright notice in the Description page of Project Settings.


#include "NSCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"


// Sets default values
ANSCharacter::ANSCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArmComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);

}

// Called when the game starts or when spawned
void ANSCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ANSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ANSCharacter::MoveForward(float value)
{
	AddMovementInput(GetActorForwardVector(), value);
}

void ANSCharacter::PrimaryAttack()
{
	FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");

	FTransform SpawnTM = FTransform(GetControlRotation(), HandLocation);

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParams);
}



// Called to bind functionality to input
void ANSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ANSCharacter::MoveForward);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);

	PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this, &ANSCharacter::PrimaryAttack);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ANSCharacter::Jump);
}

