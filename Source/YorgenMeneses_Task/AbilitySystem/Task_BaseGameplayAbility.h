
#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Task_BaseGameplayAbility.generated.h"


// Defines how ability is meant to activate
 
UENUM(BlueprintType)
enum class ETTAbilityActivationPolicy : uint8
{
	OnInputTriggered,
	WhileInputActive
};

UCLASS()
class YORGENMENESES_TASK_API UTask_BaseGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:

	ETTAbilityActivationPolicy GetActivationPolicy() const { return ActivationPolicy; }

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability Activation")
	ETTAbilityActivationPolicy ActivationPolicy;
	

};
