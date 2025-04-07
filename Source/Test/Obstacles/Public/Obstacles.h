// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PaperSpriteComponent.h"
#include "PaperSprite.h"
#include "Obstacles.generated.h"

UCLASS()
class TEST_API AObstacles : public AActor
{
    GENERATED_BODY()

public:
    AObstacles();
    void InitWithSprite(const FString& SpritePath);

protected:
    UPROPERTY(VisibleAnywhere)
    UPaperSpriteComponent* SpriteComponent;
};