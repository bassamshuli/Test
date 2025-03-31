// Fill out your copyright notice in the Description page of Project Settings.


#include "BrawlerSoldier.h"

ABrawlerSoldier::ABrawlerSoldier()
{
    PrimaryActorTick.bCanEverTick = true;
    MaxMovement = 6;
    AttackType = EAttackType::Melee;
    AttackRange = 1;
    MinDamage = 1;
    MaxDamage = 6;
    Health = 40;
}

void ABrawlerSoldier::BeginPlay() { Super::BeginPlay(); }




