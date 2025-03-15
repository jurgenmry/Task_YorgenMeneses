
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffectTypes.h"
#include "Task_PlayerState.generated.h"


class UTask_AttributeSet;
class UTask_AbilitySystemComponent;
class UAbilitySystemComponent;

UCLASS()
class YORGENMENESES_TASK_API ATask_PlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:


	ATask_PlayerState();

	// Implement IAbilitySystemInterface
	UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UFUNCTION(BlueprintCallable)
	UTask_AttributeSet* GetAttributeSetBase() const;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


	//Accessors
	UFUNCTION(BlueprintCallable, Category = "Gas|Attributes")
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Gas|Attributes")
	float GetMaxHealth() const;


protected:

	UPROPERTY()
	UTask_AbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY()
	UTask_AttributeSet* AttributeSet;

};
