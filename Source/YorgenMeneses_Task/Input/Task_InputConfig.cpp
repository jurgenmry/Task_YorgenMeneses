
#include "Input/Task_InputConfig.h"

const UInputAction* UTask_InputConfig::FindNativeInputActionForTag(const FGameplayTag& InputTag) const
{
	for (const FESInputAction& Action : NativeInputActions)
	{
		if (Action.InputAction && (Action.InputTag == InputTag))
		{
			return Action.InputAction;
		}
	}
	UE_LOG(LogTemp, Error, TEXT("Can't find NativeInputAction for InputTag [%s] on InputConfig [%s]."), *InputTag.ToString(), *GetNameSafe(this));

	return nullptr;
}

const UInputAction* UTask_InputConfig::FindAbilityInputActionForTag(const FGameplayTag& InputTag) const
{
	for (const FESInputAction& Action : AbilityInputActions)
	{
		if (Action.InputAction && (Action.InputTag == InputTag))
		{
			return Action.InputAction;
		}
	}
	UE_LOG(LogTemp, Error, TEXT("Can't find NativeInputAction for InputTag [%s] on InputConfig [%s]."), *InputTag.ToString(), *GetNameSafe(this));

	return nullptr;
}