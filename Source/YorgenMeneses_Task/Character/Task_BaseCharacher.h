
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GameplayTagContainer.h"
#include "AbilitySystemInterface.h"
#include "Task_BaseCharacher.generated.h"

class UAbilitySystemComponent;
class UTask_AbilitySystemComponent;
class UTask_AttributeSet;

UCLASS(Abstract)
class YORGENMENESES_TASK_API ATask_BaseCharacher : public APawn, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:

	ATask_BaseCharacher();

	// Implement IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
	
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo();

	UPROPERTY()
	TWeakObjectPtr<UTask_AbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UTask_AttributeSet>AttributeSet;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GAS|Abilities")
	TSubclassOf<class UGameplayEffect> VitaltAttributes;

	virtual void InitializeAttributes();

	virtual void RemoveAllGameplayEffects();

	virtual void ApplyGEToSelf(TSubclassOf<UGameplayEffect>GameplayEffectClass, float Level); //Apply effect to self

};
