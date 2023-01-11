// Fill out your copyright notice in the Description page of Project Settings.


#include "NSCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "NSInteractionComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetMathLibrary.h"
#include "TimerManager.h"
#include "DrawDebugHelpers.h"
#include "Logging/LogMacros.h"
#include "NSProjectile.h"

DECLARE_DELEGATE_OneParam(FInputSwitchAttackDelegate, const MyEnum);

// Sets default values
ANSCharacter::ANSCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->bUsePawnControlRotation = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);

	InteractionComp = CreateDefaultSubobject<UNSInteractionComponent>("InteractionComp");

	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;

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
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;

	AddMovementInput(ControlRot.Vector(), value);
}

void ANSCharacter::MoveRight(float value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;

	FVector RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);

	AddMovementInput(RightVector, value);
}

void ANSCharacter::SetAttackTimer(const MyEnum e)
{
	//FInputSwitchAttackDelegate Del;
	FTimerDelegate PDelegate;
	
	switch (e)
	{
	case MagicProjectileE: 
		
		PDelegate.BindLambda([&]()
		{
			ANSCharacter::Attack_TimeElapsed(MagicProjectile);
		});
		
		GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, PDelegate, 0.2, false);

		break;

	case BlackholeProjectileE:

		PDelegate.BindLambda([&]()
		{
			ANSCharacter::Attack_TimeElapsed(BlackholeProjectile);
		});

		GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, PDelegate, 0.2, false);
		
	case TeleportProjectileE:

		PDelegate.BindLambda([&]()
		{
			ANSCharacter::Attack_TimeElapsed(TeleportProjectile);
		});

		GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, PDelegate, 0.2, false);
		break;

	default:
		break;
	}

	PlayAnimMontage(AttackAnim);
}

void ANSCharacter::Attack_TimeElapsed(TSubclassOf<AActor> Projectile)
{

	FCollisionObjectQueryParams  ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);

	FRotator CameraRotation = CameraComp->GetComponentRotation();
	FVector CameraLocation = CameraComp->GetComponentLocation();
	FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");
	FVector End = CameraLocation + (CameraRotation.Vector() * 1000);;

	FHitResult Hit;

	bool BlockingHit = GetWorld()->LineTraceSingleByObjectType(Hit, CameraLocation, End, ObjectQueryParams);

	FTransform SpawnTM;
	FRotator MyRotation;

	if(BlockingHit)
	{
		MyRotation = UKismetMathLibrary::FindLookAtRotation(HandLocation, Hit.Location);
	}
	else
	{
		MyRotation = UKismetMathLibrary::FindLookAtRotation(HandLocation, End);
	}
	
	SpawnTM = FTransform(MyRotation, HandLocation);

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Instigator = this;

	FTimerDelegate PDelegate;

	if (Projectile == TeleportProjectile)
	{
		ANSProjectile* MyProjectile = GetWorld()->SpawnActor<ANSProjectile>(Projectile, SpawnTM, SpawnParams);
		MyProjectile->WaitForTeleport();

		PDelegate.BindLambda([&]()
			{
				ANSCharacter::Teleport(MyProjectile);
			});

		GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, PDelegate, 0.2, false);
	}
	else
	{
		GetWorld()->SpawnActor<AActor>(Projectile, SpawnTM, SpawnParams);
	}

	DrawDebugLine(GetWorld(), HandLocation, Hit.Location, FColor::Blue, false, 2.0f, 0, 2.0f);

}


void ANSCharacter::Teleport(AActor* Projectile)
{
	//GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, 0.2, false);
	//AActor DefaultBaseClassRef = Projectile->GetDefaultObject();
	//GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, PDelegate, 0.2, false);
	FString MyOb = GetNameSafe(Projectile);
	UE_LOG(LogTemp, Warning, TEXT("Projectile: %s, at game time: %f"), *MyOb, GetWorld()->TimeSeconds);

}

void ANSCharacter::PrimaryInteract()
{
	if (InteractionComp)
	{
		InteractionComp->PrimaryInteract();
	}
}


// Called to bind functionality to input
void ANSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ANSCharacter::MoveForward);

	PlayerInputComponent->BindAxis("MoveRight", this, &ANSCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction<FInputSwitchAttackDelegate>("PrimaryAttack", IE_Pressed, this, &ANSCharacter::SetAttackTimer, MagicProjectileE);

	PlayerInputComponent->BindAction<FInputSwitchAttackDelegate>("SecondaryAttack", IE_Pressed, this, &ANSCharacter::SetAttackTimer, BlackholeProjectileE);
	
	PlayerInputComponent->BindAction<FInputSwitchAttackDelegate>("Teleport", IE_Pressed, this, &ANSCharacter::SetAttackTimer, TeleportProjectileE);

	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &ANSCharacter::PrimaryInteract);


	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ANSCharacter::Jump);

}

