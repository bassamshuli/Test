// Fill out your copyright notice in the Description page of Project Settings.


#include "BrawlerSoldier.h"
#include "PaperSprite.h"
#include "PaperSpriteComponent.h"

ABrawlerSoldier::ABrawlerSoldier()
{
    SpriteComponent = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("BrawlerSprite"));
    RootComponent = SpriteComponent;

    SpriteComponent->SetRelativeRotation(FRotator(0.f, 0.f, -90.f));
    SpriteComponent->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));

    // Stats specifici
    MaxMovement = 6;
    AttackType = EAttackType::Melee;
    AttackRange = 1;
    MinDamage = 1;
    MaxDamage = 6;
    Health = 40;
}

void ABrawlerSoldier::BeginPlay()
{
    Super::BeginPlay();

    if (Team == ETeam::Player)
    {
        UPaperSprite* SpriteAsset = LoadObject<UPaperSprite>(nullptr, TEXT("/Game/Sprites/Soldier2_Green_Sprite.Soldier2_Green_Sprite"));
        if (SpriteAsset)
        {
            SpriteComponent->SetSprite(SpriteAsset);
        }
    }
    else
    {
        UPaperSprite* SpriteAsset = LoadObject<UPaperSprite>(nullptr, TEXT("/Game/Sprites/Soldier2_Red_Sprite.Soldier2_Red_Sprite"));
        if (SpriteAsset)
        {
            SpriteComponent->SetSprite(SpriteAsset);
        }
    }
}
