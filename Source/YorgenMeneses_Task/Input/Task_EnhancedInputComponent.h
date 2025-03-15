

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "Input/Task_InputConfig.h"
#include "Task_EnhancedInputComponent.generated.h"


UCLASS()
class YORGENMENESES_TASK_API UTask_EnhancedInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:

	UTask_EnhancedInputComponent();

	template<class UserClass, typename FuncType>
	void BindNativeAction(const UTask_InputConfig* InputConfig, const FGameplayTag& InputTag, ETriggerEvent TriggerEvent, UserClass* Object, FuncType Func);

	template<class UserClass, typename PressedFuncType, typename ReleasedFuncType>
	void BindAbilityActions(const UTask_InputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, TArray<uint32>& BindHandles);

	
};
template<class UserClass, typename FuncType>
void UTask_EnhancedInputComponent::BindNativeAction(const UTask_InputConfig* InputConfig, const FGameplayTag& InputTag, ETriggerEvent TriggerEvent, UserClass* Object, FuncType Func)
{
	check(InputConfig);
	if (const UInputAction* IA = InputConfig->FindNativeInputActionForTag(InputTag))
	{
		BindAction(IA, TriggerEvent, Object, Func);
	}
}

template<class UserClass, typename PressedFuncType, typename ReleasedFuncType>
void UTask_EnhancedInputComponent::BindAbilityActions(const UTask_InputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, TArray<uint32>& BindHandles)
{
	check(InputConfig);
	for (const FESInputAction& Action : InputConfig->AbilityInputActions)
	{
		if (Action.InputAction && Action.InputTag.IsValid())
		{
			if (PressedFunc)
			{
				BindHandles.Add(BindAction(Action.InputAction, ETriggerEvent::Triggered, Object, PressedFunc, Action.InputTag).GetHandle());
			}

			if (ReleasedFunc)
			{
				BindHandles.Add(BindAction(Action.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, Action.InputTag).GetHandle());
			}
		}
	}
}