// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseGameMode.h"
#include "Camera/CameraActor.h"
#include "GameFeild.h"
#include "WBP_Game.h"
#include "AISoldierController.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"

ABaseGameMode::ABaseGameMode() {}

void ABaseGameMode::BeginPlay()
{
    Super::BeginPlay();

    bool bAIControllerFound = false;
    for (TActorIterator<AAISoldierController> It(GetWorld()); It; ++It)
    {
        bAIControllerFound = true;
        break;
    }

    if (!bAIControllerFound)
    {
        FActorSpawnParameters SpawnParams;
        GetWorld()->SpawnActor<AAISoldierController>(AAISoldierController::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
    }

    for (TActorIterator<AGameFeild> It(GetWorld()); It; ++It)
    {
        GameUIInstance = It->GameUIInstance;
        Tiles = It->Tiles;
        break;
    }

    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (PlayerController)
    {
        for (TActorIterator<ACameraActor> It(GetWorld()); It; ++It)
        {
            if (It->GetName().Contains("BP_Camera"))
            {
                PlayerController->SetViewTargetWithBlend(*It, 0.0f);
                break;
            }
        }
    }
}

void ABaseGameMode::StartGame()
{
    CurrentUnitIndex = 0;
    bIsPlayerTurn = FMath::RandBool();

    if (bIsPlayerTurn)
    {
        if (GameUIInstance)
        {
            GameUIInstance->ShowChooseUnitTypeUI();
        }
    }
    else
    {
        SpawnQueue = { BP_Brawler_Red, BP_Brawler_Green, BP_Sniper_Red, BP_Sniper_Green };
        GetWorldTimerManager().SetTimerForNextTick(this, &ABaseGameMode::NextTurn);
    }
}

void ABaseGameMode::PlayerChoseStartingUnit(bool bBrawlerFirst)
{
    if (bBrawlerFirst)
    {
        SpawnQueue = { BP_Brawler_Green, BP_Brawler_Red, BP_Sniper_Green, BP_Sniper_Red };
    }
    else
    {
        SpawnQueue = { BP_Sniper_Green, BP_Sniper_Red, BP_Brawler_Green, BP_Brawler_Red };
    }

    CurrentUnitIndex = 0;
    if (GameUIInstance)
    {
        GameUIInstance->ShowPlacementMessage(true, CurrentUnitIndex);
    }
}

void ABaseGameMode::NextTurn()
{
    if (CurrentUnitIndex >= SpawnQueue.Num()) return;

    if (bIsPlayerTurn)
    {
        for (ATile* Tile : Tiles)
        {
            if (Tile)
            {
                Tile->PlayerSoldierToSpawn = SpawnQueue[CurrentUnitIndex];
            }
        }
    }

    if (GameUIInstance)
    {
        GameUIInstance->ShowPlacementMessage(bIsPlayerTurn, CurrentUnitIndex);
    }

    if (!bIsPlayerTurn)
    {
        GetWorldTimerManager().SetTimerForNextTick([this]() {
            for (TActorIterator<AAISoldierController> It(GetWorld()); It; ++It)
            {
                It->PlaceAIUnitDelayed(Tiles, SpawnQueue, CurrentUnitIndex, bIsPlayerTurn, GameUIInstance);
                break;
            }
            });
    }
}

void ABaseGameMode::HandleTileClicked(ATile* ClickedTile)
{
    if (!ClickedTile || !ClickedTile->IsTileFree() || !bIsPlayerTurn) return;

    FVector SpawnLocation = ClickedTile->GetActorLocation() + FVector(0, 0, 50);
    ASoldier* PlayerUnit = GetWorld()->SpawnActor<ASoldier>(SpawnQueue[CurrentUnitIndex], SpawnLocation, FRotator::ZeroRotator);

    if (PlayerUnit)
    {
        ClickedTile->SetTileOccupied(true);
        CurrentUnitIndex++;
        bIsPlayerTurn = false;
        NextTurn();
    }
}
