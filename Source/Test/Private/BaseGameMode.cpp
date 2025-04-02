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

    // Spawna AI Controller se non presente
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
        UE_LOG(LogTemp, Warning, TEXT("🤖 AAISoldierController spawnato dinamicamente."));
    }

    for (TActorIterator<AGameFeild> It(GetWorld()); It; ++It)
    {
        UWBP_Game* UI = It->GameUIInstance;
        if (UI)
        {
            GameUIInstance = UI;
            break;
        }
    }

    for (TActorIterator<AGameFeild> It(GetWorld()); It; ++It)
    {
        GameUIInstance = It->GameUIInstance;
        Tiles = It->Tiles;
        break;
    }


    UWorld* World = GetWorld();
    if (World)
    {
        APlayerController* PlayerController = UGameplayStatics::GetPlayerController(World, 0);
        if (PlayerController)
        {
            for (TActorIterator<ACameraActor> It(World); It; ++It)
            {
                if (It->GetName().Contains("BP_Camera"))
                {
                    PlayerController->SetViewTargetWithBlend(*It, 0.0f);
                    UE_LOG(LogTemp, Warning, TEXT("✅ Camera impostata con successo da BeginPlay!"));
                    break;
                }
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
        SpawnQueue = { BP_Brawler_Green, BP_Brawler_Red, BP_Sniper_Green, BP_Sniper_Red };
    }
    else
    {
        SpawnQueue = { BP_Brawler_Red, BP_Brawler_Green, BP_Sniper_Red, BP_Sniper_Green };
    }

    UE_LOG(LogTemp, Warning, TEXT("🎯 SpawnQueue contiene %d elementi"), SpawnQueue.Num());
    for (int32 i = 0; i < SpawnQueue.Num(); ++i)
    {
        UE_LOG(LogTemp, Warning, TEXT("➡️ Index %d: %s"), i, *GetNameSafe(SpawnQueue[i]));
    }

    if (GameUIInstance)
    {
        GameUIInstance->ShowPlacementMessage(bIsPlayerTurn, CurrentUnitIndex);
    }

    if (!bIsPlayerTurn)
    {
        GetWorldTimerManager().SetTimerForNextTick(this, &ABaseGameMode::NextTurn);
    }
}

void ABaseGameMode::NextTurn()
{
    UE_LOG(LogTemp, Warning, TEXT("🔁 NextTurn chiamato - Turno: %s - CurrentUnitIndex: %d"),
        bIsPlayerTurn ? TEXT("Player") : TEXT("AI"),
        CurrentUnitIndex);

    if (CurrentUnitIndex >= SpawnQueue.Num()) return;

    if (bIsPlayerTurn && CurrentUnitIndex < SpawnQueue.Num())
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
        GetWorldTimerManager().SetTimerForNextTick([this]()
            {
                // Trova il controller AI
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

        // Passa il turno all'IA
        bIsPlayerTurn = false;
        NextTurn();
    }
}

