// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseGameMode.h"
#include "Camera/CameraActor.h"
#include "GameFeild.h"
#include "WBP_Game.h"
#include "AISoldierController.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "SniperSoldier.h"
#include "BrawlerSoldier.h"

ABaseGameMode::ABaseGameMode()
{
    SpawnQueue.Empty();
}

void ABaseGameMode::BeginPlay()
{
    Super::BeginPlay();

    if (!TActorIterator<AAISoldierController>(GetWorld()))
    {
        GetWorld()->SpawnActor<AAISoldierController>(AAISoldierController::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
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
    StartingTeam = bIsPlayerTurn ? ETeam::Player : ETeam::AI;

    if (bIsPlayerTurn)
    {
        if (GameUIInstance)
        {
            GameUIInstance->ShowChooseUnitTypeUI();
        }
    }
    else
    {
        SetupAISpawnQueue();
        if (GameUIInstance)
        {
            GameUIInstance->SetSpawnQueue(SpawnQueue);
            GameUIInstance->ShowPlacementMessage(false, CurrentUnitIndex);
        }
        GetWorldTimerManager().SetTimerForNextTick(this, &ABaseGameMode::NextTurn);
    }
}

void ABaseGameMode::SetupAISpawnQueue()
{
    bool bBrawlerFirst = FMath::RandBool();
    TSubclassOf<ASoldier> BrawlerClass = ABrawlerSoldier::StaticClass();
    TSubclassOf<ASoldier> SniperClass = ASniperSoldier::StaticClass();

    if (bBrawlerFirst)
    {
        SpawnQueue = { BrawlerClass, BrawlerClass, SniperClass, SniperClass };
    }
    else
    {
        SpawnQueue = { SniperClass, SniperClass, BrawlerClass, BrawlerClass };
    }
}

void ABaseGameMode::PlayerChoseStartingUnit(bool bBrawlerFirst)
{
    TSubclassOf<ASoldier> BrawlerClass = ABrawlerSoldier::StaticClass();
    TSubclassOf<ASoldier> SniperClass = ASniperSoldier::StaticClass();

    if (bBrawlerFirst)
    {
        SpawnQueue = { BrawlerClass, BrawlerClass, SniperClass, SniperClass };
    }
    else
    {
        SpawnQueue = { SniperClass, SniperClass, BrawlerClass, BrawlerClass };
    }

    CurrentUnitIndex = 0;

    if (GameUIInstance)
    {
        GameUIInstance->SetSpawnQueue(SpawnQueue);
        GameUIInstance->ShowPlacementMessage(true, CurrentUnitIndex);
    }

    NextTurn();
}

void ABaseGameMode::NextTurn()
{
    if (CurrentUnitIndex >= SpawnQueue.Num())
    {
        OnPlacementPhaseComplete();
        return;
    }

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
        GameUIInstance->SetSpawnQueue(SpawnQueue);
        GameUIInstance->ShowPlacementMessage(bIsPlayerTurn, CurrentUnitIndex);
    }

    if (!bIsPlayerTurn)
    {
        GetWorldTimerManager().SetTimerForNextTick([this]() {
            for (TActorIterator<AAISoldierController> It(GetWorld()); It; ++It)
            {
                It->PlaceAIUnitDelayed(Tiles, SpawnQueue, GameUIInstance, this);
                break;
            }
            });
    }
}

void ABaseGameMode::OnPlacementPhaseComplete()
{
    bActionPhaseStarted = true;
    CurrentTurnTeam = StartingTeam;

    if (GameUIInstance)
    {
        FString Message = CurrentTurnTeam == ETeam::Player ? TEXT("🎯 Player turn") : TEXT("🤖 AI turn");
        GameUIInstance->UpdateStatusMessage(FText::FromString(Message));
    }
}

void ABaseGameMode::HandleTileClicked(ATile* ClickedTile)
{
    UE_LOG(LogTemp, Warning, TEXT("🎯 bIsPlayerTurn: %s - CurrentUnitIndex: %d"), bIsPlayerTurn ? TEXT("true") : TEXT("false"), CurrentUnitIndex);
    if (CurrentUnitIndex < SpawnQueue.Num() && bIsPlayerTurn && ClickedTile->IsTileFree())
    {
        FVector SpawnLocation = ClickedTile->GetActorLocation() + FVector(0, 0, 50);
        ASoldier* NewSoldier = GetWorld()->SpawnActor<ASoldier>(SpawnQueue[CurrentUnitIndex], SpawnLocation, FRotator::ZeroRotator);
        if (!SpawnQueue.IsValidIndex(CurrentUnitIndex) || !*SpawnQueue[CurrentUnitIndex])
        {
            UE_LOG(LogTemp, Error, TEXT("❌ SpawnQueue[%d] è nullptr o non valido"), CurrentUnitIndex);
            return;
        }
        if (NewSoldier)
        {
            NewSoldier->Team = ETeam::Player;
            UE_LOG(LogTemp, Warning, TEXT("✅ Soldier spawnato correttamente: %s"), *NewSoldier->GetName());
            ClickedTile->SetTileOccupied(true);
            NewSoldier->TryAssignOwningTile(Tiles);
            CurrentUnitIndex++;

            if (CurrentUnitIndex < SpawnQueue.Num())
            {
                bIsPlayerTurn = false;
                NextTurn();
            }
            else
            {
                OnPlacementPhaseComplete();
            }
        }
    }
}

void ABaseGameMode::HandleSoldierSelected(ASoldier* Soldier)
{
    if (!bIsPlayerTurn || !Soldier) return;

    if (SelectedSoldier && SelectedSoldier->OwningTile)
    {
        SelectedSoldier->OwningTile->SetSelected(false);
    }

    for (ATile* Tile : Tiles)
    {
        if (Tile)
        {
            Tile->SetSelected(false);
        }
    }

    SelectedSoldier = Soldier;

    if (SelectedSoldier->OwningTile)
    {
        SelectedSoldier->OwningTile->SetSelected(true);
    }

    if (SelectedSoldier->Team == ETeam::Player)
    {
        SelectedSoldier->ShowMovableTiles(Tiles);
    }
}

void ABaseGameMode::ResetGame()
{
    for (TActorIterator<ASoldier> It(GetWorld()); It; ++It)
    {
        It->Destroy();
    }

    for (TActorIterator<AObstacles> It(GetWorld()); It; ++It)
    {
        It->Destroy();
    }

    for (ATile* Tile : Tiles)
    {
        if (Tile)
        {
            Tile->ResetTile();
        }
    }

    for (TActorIterator<AGameFeild> It(GetWorld()); It; ++It)
    {
        It->GenerateObstacles();
        break;
    }

    bIsPlayerTurn = true;
    CurrentUnitIndex = 0;
    bActionPhaseStarted = false;
    SelectedSoldier = nullptr;
    SelectedSoldier_Current = nullptr;
    SpawnQueue.Empty();

    if (GameUIInstance)
    {
        GameUIInstance->ShowWelcomeMessage();
        GameUIInstance->SetSpawnQueue(SpawnQueue);
    }

    UE_LOG(LogTemp, Warning, TEXT("♻️ Reset completato!"));
}