// Fill out your copyright notice in the Description page of Project Settings.


#include "PrototypeXCharacter.h"

// Sets default values
APrototypeXCharacter::APrototypeXCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APrototypeXCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APrototypeXCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APrototypeXCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

