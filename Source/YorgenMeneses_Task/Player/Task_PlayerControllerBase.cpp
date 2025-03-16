
#include "Player/Task_PlayerControllerBase.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Character/Task_BaseCharacher.h"
#include "AbilitySystem/Task_AbilitySystemComponent.h"
#include "Player/Task_PlayerState.h"

#include "Blueprint/UserWidget.h"
#include "Widgets/Task_ScoreHud.h"

ATask_PlayerControllerBase::ATask_PlayerControllerBase()
{
}

void ATask_PlayerControllerBase::BeginPlay()
{
	// Create the score widget and add it to the viewport.
	if (ScoreHudClass)
	{
		ScoreHud = CreateWidget<UTask_ScoreHud>(this, ScoreHudClass);
		if (ScoreHud)
		{
			ScoreHud->AddToViewport();
		}
	}
}



ATask_BaseCharacher* ATask_PlayerControllerBase::GetPlayer() const
{
	return CastChecked<ATask_BaseCharacher>(GetPawn(), ECastCheckedType::NullAllowed);;
}

ATask_PlayerState* ATask_PlayerControllerBase::GetPlayerState() const
{
	return CastChecked<ATask_PlayerState>(PlayerState, ECastCheckedType::NullAllowed);
}

UTask_AbilitySystemComponent* ATask_PlayerControllerBase::GetAbilitySystemComponent() const
{
	/*const ATask_BaseCharacher* BasePlayer = GetPlayer();
	if (BasePlayer)
	{
		return CastChecked<UTask_AbilitySystemComponent>(BasePlayer->GetAbilitySystemComponent());
	}
	else
	{
		return nullptr;
	}*/


	const ATask_PlayerState* playerState = GetPlayerState();
	if (playerState)
	{
		return CastChecked<UTask_AbilitySystemComponent>(playerState->GetAbilitySystemComponent());
	}
	else
	{
		return nullptr;
	}

}

void ATask_PlayerControllerBase::PreProcessInput(const float DeltaTime, const bool bGamePaused)
{
	Super::PreProcessInput(DeltaTime, bGamePaused);
}

void ATask_PlayerControllerBase::PostProcessInput(const float DeltaTime, const bool bGamePaused)
{
	if (UTask_AbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		ASC->ProcessAbilityInput(DeltaTime, bGamePaused);
	}

	Super::PostProcessInput(DeltaTime, bGamePaused);
}

void ATask_PlayerControllerBase::AddPointsToHUD(int32 Points)
{
	if (ScoreHud)
	{
		ScoreHud->AddPoints(Points);
	}
}
