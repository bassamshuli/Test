// Fill out your copyright notice in the Description page of Project Settings.


#include "Obstacles.h"
#include "UObject/ConstructorHelpers.h"

AObstacles::AObstacles()
{
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    SpriteComponent = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("SpriteComponent"));
    SpriteComponent->SetRelativeRotation(FRotator(0.f, 0.f, -90.f));
    SpriteComponent->SetupAttachment(RootComponent);
    SpriteComponent->SetCollisionProfileName(TEXT("BlockAll"));
}

void AObstacles::InitWithSprite(const FString& SpritePath)
{
    ConstructorHelpers::FObjectFinder<UPaperSprite> SpriteAsset(*SpritePath);
    if (SpriteAsset.Succeeded())
    {
        SpriteComponent->SetSprite(SpriteAsset.Object);
    }
}