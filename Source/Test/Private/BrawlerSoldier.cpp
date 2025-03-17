// Fill out your copyright notice in the Description page of Project Settings.


#include "BrawlerSoldier.h"

// Sets default values
ABrawlerSoldier::ABrawlerSoldier()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABrawlerSoldier::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABrawlerSoldier::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABrawlerSoldier::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

