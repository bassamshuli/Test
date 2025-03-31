// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Soldier.h"
#include "SniperSoldier.generated.h"

UCLASS()
class TEST_API ASniperSoldier : public ASoldier
{
    GENERATED_BODY()

public:
    ASniperSoldier();
    virtual void BeginPlay() override;
};