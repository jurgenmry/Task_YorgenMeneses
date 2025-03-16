// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Task_ScoreHud.generated.h"

class UTextBlock;

UCLASS()
class YORGENMENESES_TASK_API UTask_ScoreHud : public UUserWidget
{
	GENERATED_BODY()
	
public:
	
	UFUNCTION(BlueprintCallable, Category = "Score")
	void AddPoints(int32 Points);

protected:
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ScoreText;

private:
	int32 CurrentScore = 0;
};
