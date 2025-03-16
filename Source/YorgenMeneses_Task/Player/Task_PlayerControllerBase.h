
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameplayTagContainer.h"
#include "InputActionValue.h"
#include "Character/Task_BaseCharacher.h"
#include "Task_PlayerControllerBase.generated.h"


class UTask_AbilitySystemComponent;
class UTask_ScoreHud;

UCLASS()
class YORGENMENESES_TASK_API ATask_PlayerControllerBase : public APlayerController
{
	GENERATED_BODY()

public:


	ATask_PlayerControllerBase();
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Score")
	void AddPointsToHUD(int32 Points);

protected:

	UFUNCTION(BlueprintCallable, Category = "InputSystem|Player")
	ATask_BaseCharacher* GetPlayer() const;

	UFUNCTION(BlueprintCallable, Category = "InputSystem|PlayerState")
	ATask_PlayerState* GetPlayerState() const;

	UFUNCTION(BlueprintCallable, Category = "InputSystem|AbilitySystemComponent")
	UTask_AbilitySystemComponent* GetAbilitySystemComponent() const;

	virtual void PreProcessInput(const float DeltaTime, const bool bGamePaused) override;
	virtual void PostProcessInput(const float DeltaTime, const bool bGamePaused) override;


	// The widget class set in the editor (or defaults) that represents the score HUD.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UTask_ScoreHud> ScoreHudClass;

	UTask_ScoreHud* ScoreHud;



};
