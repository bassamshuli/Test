// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerSoldier.h"

// Sets default values
APlayerSoldier::APlayerSoldier()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APlayerSoldier::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerSoldier::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerSoldier::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

