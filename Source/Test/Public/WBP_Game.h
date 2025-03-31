// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "WBP_Game.generated.h"

UCLASS()
class TEST_API UWBP_Game : public UUserWidget
{
    GENERATED_BODY()

public:
    virtual bool Initialize() override;

    UFUNCTION(BlueprintCallable, Category = "UI") void UpdateStatusMessage(const FText& NewMessage);
    UFUNCTION() void StartGameButtonClicked();

    UPROPERTY(meta = (BindWidget)) UTextBlock* StatusText;
    UPROPERTY(meta = (BindWidget)) UButton* StartButton;
};