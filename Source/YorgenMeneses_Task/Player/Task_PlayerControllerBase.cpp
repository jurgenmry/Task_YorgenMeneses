
#include "Player/Task_PlayerControllerBase.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Character/Task_BaseCharacher.h"
#include "AbilitySystem/Task_AbilitySystemComponent.h"

ATask_PlayerControllerBase::ATask_PlayerControllerBase()
{
}



ATask_BaseCharacher* ATask_PlayerControllerBase::GetPlayer() const
{
	return CastChecked<ATask_BaseCharacher>(GetCharacter(), ECastCheckedType::NullAllowed);;
}

UTask_AbilitySystemComponent* ATask_PlayerControllerBase::GetAbilitySystemComponent() const
{
	const ATask_BaseCharacher* BasePlayer = GetPlayer();
	if (BasePlayer)
	{
		return CastChecked<UTask_AbilitySystemComponent>(BasePlayer->GetAbilitySystemComponent());
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
