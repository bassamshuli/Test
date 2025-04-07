// Fill out your copyright notice in the Description page of Project Settings.


#include "BrawlerSoldier.h"
#include "PaperSprite.h"
#include "PaperSpriteComponent.h"

ABrawlerSoldier::ABrawlerSoldier()
{
    static ConstructorHelpers::FObjectFinder<UPaperSprite> DefaultSprite(TEXT("/Game/Sprites/Soldier1_Green_Sprite.Soldier1_Green_Sprite"));
    if (DefaultSprite.Succeeded())
    {
        SpriteComponent->SetSprite(DefaultSprite.Object);
    }

    SpriteComponent->SetRelativeScale3D(FVector(0.4f, 0.4f, 0.4f));

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