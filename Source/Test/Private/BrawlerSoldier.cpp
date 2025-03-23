// Fill out your copyright notice in the Description page of Project Settings.


#include "BrawlerSoldier.h"

// Sets default values
ABrawlerSoldier::ABrawlerSoldier()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    MaxMovement = 6;
    AttackType = EAttackType::Melee;
    AttackRange = 1;
    MinDamage = 1;
    MaxDamage = 6;
    Health = 40;
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



