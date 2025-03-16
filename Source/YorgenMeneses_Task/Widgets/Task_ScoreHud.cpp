// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Task_ScoreHud.h"
#include "Components/TextBlock.h"

void UTask_ScoreHud::AddPoints(int32 Points)
{
    CurrentScore += Points;

    // Update the UI text if the TextBlock is valid.
    if (ScoreText)
    {
        ScoreText->SetText(FText::AsNumber(CurrentScore));
    }
}
