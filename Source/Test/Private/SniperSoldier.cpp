// Fill out your copyright notice in the Description page of Project Settings.


#include "SniperSoldier.h"

// Sets default values
ASniperSoldier::ASniperSoldier()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASniperSoldier::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASniperSoldier::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASniperSoldier::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

