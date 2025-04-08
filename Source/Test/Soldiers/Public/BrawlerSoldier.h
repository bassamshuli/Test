// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Soldier.h"
#include "BrawlerSoldier.generated.h"

UCLASS()
class TEST_API ABrawlerSoldier : public ASoldier
{
    GENERATED_BODY()

public:
    ABrawlerSoldier();

protected:
    virtual void BeginPlay() override;
};