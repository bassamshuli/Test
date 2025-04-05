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
    if (bBrawlerFirst)
    {
        SpawnQueue = { BP_Brawler_Red, BP_Brawler_Green, BP_Sniper_Red, BP_Sniper_Green };
    }
    else
    {
        SpawnQueue = { BP_Sniper_Red, BP_Sniper_Green, BP_Brawler_Red, BP_Brawler_Green };
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
        FString Message = CurrentTurnTeam == ETeam::Player
            ? TEXT("🎯 Player turn")
            : TEXT("🤖 AI turn");
        GameUIInstance->UpdateStatusMessage(FText::FromString(Message));
    }
}

void ABaseGameMode::HandleTileClicked(ATile* ClickedTile)
{
    if (CurrentUnitIndex < SpawnQueue.Num() && bIsPlayerTurn && ClickedTile->IsTileFree())
    {
        FVector SpawnLocation = ClickedTile->GetActorLocation() + FVector(0, 0, 50);
        ASoldier* NewSoldier = GetWorld()->SpawnActor<ASoldier>(SpawnQueue[CurrentUnitIndex], SpawnLocation, FRotator::ZeroRotator);

        if (NewSoldier)
        {
            ClickedTile->SetTileOccupied(true);
            NewSoldier->Team = ETeam::Player;
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
            return;
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
    // 1. Cancella tutti i soldati
    for (TActorIterator<ASoldier> It(GetWorld()); It; ++It)
    {
        It->Destroy();
    }

    // 2. Cancella tutti gli ostacoli
    for (TActorIterator<AObstacles> It(GetWorld()); It; ++It)
    {
        It->Destroy();
    }

    // 3. Pulisce le tile esistenti (reset)
    for (ATile* Tile : Tiles)
    {
        if (Tile)
        {
            Tile->SetTileOccupied(false);
            Tile->bHasObstacle = false;
            Tile->SetSelected(false);
        }
    }

    // 4. Rigenera nuovi ostacoli
    for (TActorIterator<AGameFeild> It(GetWorld()); It; ++It)
    {
        It->GenerateObstacles(); // 💡 chiamata alla funzione già esistente
        break;
    }

    // 5. Reset variabili interne
    bIsPlayerTurn = true;
    CurrentUnitIndex = 0;
    bActionPhaseStarted = false;
    SelectedSoldier = nullptr;
    SelectedSoldier_Current = nullptr;
    SpawnQueue.Empty();

    // 6. Mostra messaggio di benvenuto e resetta UI
    if (GameUIInstance)
    {
        GameUIInstance->ShowWelcomeMessage(); // mostra messaggio
        GameUIInstance->SetSpawnQueue(SpawnQueue); // svuota coda
    }

    UE_LOG(LogTemp, Warning, TEXT("♻️ Reset completato!"));
}