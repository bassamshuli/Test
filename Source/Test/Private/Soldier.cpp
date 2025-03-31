// Fill out your copyright notice in the Description page of Project Settings.


#include "Soldier.h"
#include "Math/UnrealMathUtility.h"

ASoldier::ASoldier() { PrimaryActorTick.bCanEverTick = true; }

void ASoldier::BeginPlay() { Super::BeginPlay(); }
void ASoldier::Tick(float DeltaTime) { Super::Tick(DeltaTime); }
int32 ASoldier::GetRandomDamage() const { return FMath::RandRange(MinDamage, MaxDamage); }
