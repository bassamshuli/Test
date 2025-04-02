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
}

void UWBP_Game::ShowWelcomeMessage()
{
    UpdateStatusMessage(FText::FromString(TEXT("👋 Benvenuto! Premi Start per iniziare")));
}

void UWBP_Game::ShowPlacementMessage(bool bIsPlayerTurn, int32 CurrentUnitIndex)
{
    FString Message;

    if (CurrentUnitIndex <= 1)
        Message = bIsPlayerTurn ? TEXT("🎯 Player turn - Posiziona il tuo BRAWLER") : TEXT("🤖 AI turn - Posiziona il suo BRAWLER");
    else
        Message = bIsPlayerTurn ? TEXT("🎯 Player turn - Posiziona il tuo SNIPER") : TEXT("🤖 AI turn - Posiziona il suo SNIPER");

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
    // Non serve cancellare lo StatusText, sarà sovrascritto da ShowPlacementMessage()
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