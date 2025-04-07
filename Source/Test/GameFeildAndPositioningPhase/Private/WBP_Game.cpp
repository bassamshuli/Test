// Fill out your copyright notice in the Description page of Project Settings.

#include "WBP_Game.h"
#include "Kismet/GameplayStatics.h"
#include "BaseGameMode.h"
#include "GameFeild.h"

bool UWBP_Game::Initialize()
{
    const bool Success = Super::Initialize();
    if (!Success) return false;

    if (StartButton)
    {
        StartButton->OnClicked.AddUniqueDynamic(this, &UWBP_Game::StartGameButtonClicked);
        StartButton->SetVisibility(ESlateVisibility::Visible); // Mostra Start
    }

    if (ResetButton)
    {
        ResetButton->OnClicked.AddUniqueDynamic(this, &UWBP_Game::ResetGameButtonClicked);
        ResetButton->SetVisibility(ESlateVisibility::Collapsed); // Nascondi Reset
    }

    if (ButtonChooseBrawler)
    {
        ButtonChooseBrawler->OnClicked.AddDynamic(this, &UWBP_Game::OnBrawlerChosen);
    }

    if (ButtonChooseSniper)
    {
        ButtonChooseSniper->OnClicked.AddDynamic(this, &UWBP_Game::OnSniperChosen);
    }

    GameModeRef = Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(this));

    return true;
}

void UWBP_Game::StartGameButtonClicked()
{
    if (GameModeRef)
    {
        GameModeRef->StartGame();
    }

    // 🔁 Cambio visibilità pulsanti
    if (StartButton)
        StartButton->SetVisibility(ESlateVisibility::Collapsed);

    if (ResetButton)
        ResetButton->SetVisibility(ESlateVisibility::Visible);
}

void UWBP_Game::ResetGameButtonClicked()
{
    if (GameModeRef)
    {
        GameModeRef->ResetGame(); // ⚠️ assicurati che questa funzione esista
    }

    // Torna allo stato iniziale
    if (StartButton)
        StartButton->SetVisibility(ESlateVisibility::Visible);

    if (ResetButton)
        ResetButton->SetVisibility(ESlateVisibility::Collapsed);
}

void UWBP_Game::ShowWelcomeMessage()
{
    UpdateStatusMessage(FText::FromString(TEXT("👋 Benvenuto! Premi Start per iniziare")));
}

void UWBP_Game::ShowPlacementMessage(bool bIsPlayerTurn, int32 CurrentUnitIndex)
{
    FString Message;

    FString UnitName = SpawnQueue.IsValidIndex(CurrentUnitIndex) ? SpawnQueue[CurrentUnitIndex]->GetName() : TEXT("Unità");
    bool bIsBrawler = UnitName.Contains("Brawler");

    if (bIsPlayerTurn)
        Message = FString::Printf(TEXT("🎯 Player turn - Posiziona il tuo %s"), bIsBrawler ? TEXT("BRAWLER") : TEXT("SNIPER"));
    else
        Message = FString::Printf(TEXT("🤖 AI turn - Posiziona il suo %s"), bIsBrawler ? TEXT("BRAWLER") : TEXT("SNIPER"));

    UpdateStatusMessage(FText::FromString(Message));
}

void UWBP_Game::ShowChooseUnitTypeUI()
{
    UE_LOG(LogTemp, Warning, TEXT("✅ ShowChooseUnitTypeUI() chiamato"));

    UpdateStatusMessage(FText::FromString(TEXT("🟢 Scegli quale tipo vuoi posizionare per primo")));

    if (ButtonChooseBrawler)
    {
        UE_LOG(LogTemp, Warning, TEXT("✅ ButtonChooseBrawler trovato"));
        ButtonChooseBrawler->SetVisibility(ESlateVisibility::Visible);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("❌ ButtonChooseBrawler è nullptr"));
    }

    if (ButtonChooseSniper)
    {
        UE_LOG(LogTemp, Warning, TEXT("✅ ButtonChooseSniper trovato"));
        ButtonChooseSniper->SetVisibility(ESlateVisibility::Visible);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("❌ ButtonChooseSniper è nullptr"));
    }
}

void UWBP_Game::HideChooseButtons()
{
    if (ButtonChooseBrawler)
        ButtonChooseBrawler->SetVisibility(ESlateVisibility::Collapsed);

    if (ButtonChooseSniper)
        ButtonChooseSniper->SetVisibility(ESlateVisibility::Collapsed);
}

void UWBP_Game::OnBrawlerChosen()
{
    HideChooseButtons();
    if (GameModeRef)
        GameModeRef->PlayerChoseStartingUnit(true);
}

void UWBP_Game::OnSniperChosen()
{
    HideChooseButtons();
    if (GameModeRef)
        GameModeRef->PlayerChoseStartingUnit(false);
}

void UWBP_Game::UpdateStatusMessage(const FText& NewMessage)
{
    if (StatusText)
    {
        StatusText->SetText(NewMessage);
    }
}

void UWBP_Game::SetSpawnQueue(const TArray<TSubclassOf<class ASoldier>>& InQueue)
{
    SpawnQueue = InQueue;
}