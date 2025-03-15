

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayAbilitySpecHandle.h"
#include "GameplayTagContainer.h"
#include "Task_AbilitySet.generated.h"

class UTask_AbilitySystemComponent;
class UTask_BaseGameplayAbility;

USTRUCT(BlueprintType)
struct FSAbilitySet_GameplayAbility
{
	GENERATED_BODY()

public:

	// Gameplay ability to grant.
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UTask_AbilitySystemComponent> Ability = nullptr;

	// Level of ability to grant.
	UPROPERTY(EditDefaultsOnly)
	int32 AbilityLevel = 1;

	// Tag used to process input for the ability.
	UPROPERTY(EditDefaultsOnly, Meta = (Categories = "InputTag"))
	FGameplayTag InputTag;
};

USTRUCT(BlueprintType)
struct FSAbilitySet_GrantedHandles
{
	GENERATED_BODY()

public:
void AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle);

protected:
	UPROPERTY()
	TArray<FGameplayAbilitySpecHandle> AbilitySpecHandles;
};

UCLASS()
class YORGENMENESES_TASK_API UTask_AbilitySet : public UDataAsset
{
	GENERATED_BODY()

public:

	// Grants the ability set to the specified ability system component.
	// The returned handles can be used later to take away anything that was granted.
	void GiveToAbilitySystem(UTask_AbilitySystemComponent* InASC, FSAbilitySet_GrantedHandles* OutGrantedHandles, UObject* SourceObject = nullptr) const;

protected:

	// Gameplay abilities to grant when this ability set is granted.
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Abilities", meta = (TitleProperty = Ability))
	TArray<FSAbilitySet_GameplayAbility> GrantedGameplayAbilities;
	
};
