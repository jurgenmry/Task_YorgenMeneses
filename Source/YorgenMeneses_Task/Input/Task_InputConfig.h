
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "Task_InputConfig.generated.h"

class UInputAction;
//FISInputAction
//Struct used to map an input action to a gameplay input tag.
 
USTRUCT(BlueprintType)
struct FESInputAction
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly)
	const UInputAction* InputAction = nullptr;

	UPROPERTY(EditDefaultsOnly, Meta = (Categories = "InputTag"))
	FGameplayTag InputTag;
};

UCLASS()
class YORGENMENESES_TASK_API UTask_InputConfig : public UDataAsset
{
	GENERATED_BODY()

public:

	const UInputAction* FindNativeInputActionForTag(const FGameplayTag& InputTag) const;
	const UInputAction* FindAbilityInputActionForTag(const FGameplayTag& InputTag) const;

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (TitleProperty = "InputAction"))
	TArray<FESInputAction> NativeInputActions;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (TitleProperty = "InputAction"))
	TArray<FESInputAction> AbilityInputActions;
	
};
