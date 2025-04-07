// Fill out your copyright notice in the Description page of Project Settings.


#include "SniperSoldier.h"

ASniperSoldier::ASniperSoldier()
{
    PrimaryActorTick.bCanEverTick = true;
    MaxMovement = 3;
    AttackType = EAttackType::Ranged;
    AttackRange = 10;
    MinDamage = 4;
    MaxDamage = 8;
    Health = 20;
}

void ASniperSoldier::BeginPlay() { Super::BeginPlay(); }



