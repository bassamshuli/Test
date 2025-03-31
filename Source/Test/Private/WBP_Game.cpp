// Fill out your copyright notice in the Description page of Project Settings.


#include "WBP_Game.h"
#include "Kismet/GameplayStatics.h"
#include "GameFeild.h"

bool UWBP_Game::Initialize()
{
    const bool Success = Super::Initialize();
    if (!Success) return false;

    if (StartButton)
    {
        StartButton->OnClicked.AddUniqueDynamic(this, &UWBP_Game::StartGameButtonClicked);
    }

    return true;
}

void UWBP_Game::StartGameButtonClicked()
{
    AGameFeild* GameFeild = Cast<AGameFeild>(UGameplayStatics::GetActorOfClass(GetWorld(), AGameFeild::StaticClass()));
    if (GameFeild)
    {
        GameFeild->StartGame();
    }
}

void UWBP_Game::UpdateStatusMessage(const FText& NewMessage)
{
    if (StatusText)
    {
        StatusText->SetText(NewMessage);
    }
}