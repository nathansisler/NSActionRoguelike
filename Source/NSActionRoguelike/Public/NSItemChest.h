// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NSGameplayInterface.h"
#include "NSItemChest.generated.h"

class UStaticMeshComponent;

UCLASS()
class NSACTIONROGUELIKE_API ANSItemChest : public AActor, public INSGameplayInterface 
{
	GENERATED_BODY()

	void Interact_Implementation(APawn* InstigatorPawn);

protected:

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BaseMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* LidMesh;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Sets default values for this actor's properties
	ANSItemChest();
	
	UPROPERTY(EditAnywhere)
	float TargetPitch;

};
