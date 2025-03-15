
#include "Player/Task_PlayerState.h"

#include "AbilitySystem/Task_AttributeSet.h"
#include "AbilitySystem/Task_AbilitySystemComponent.h"
#include "Net/UnrealNetwork.h"

ATask_PlayerState::ATask_PlayerState()
{
    AbilitySystemComponent = CreateDefaultSubobject<UTask_AbilitySystemComponent>(TEXT("AbilitySystemComponent"));
    AbilitySystemComponent->SetIsReplicated(true);
    AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
    AttributeSet = CreateDefaultSubobject<UTask_AttributeSet>(TEXT("AttributeSet"));

    NetUpdateFrequency = 100.0f;
}

UAbilitySystemComponent* ATask_PlayerState::GetAbilitySystemComponent() const
{
    return AbilitySystemComponent;
}

UTask_AttributeSet* ATask_PlayerState::GetAttributeSetBase() const
{
    return AttributeSet;
}

void ATask_PlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

float ATask_PlayerState::GetHealth() const
{
    return AttributeSet->GetHealth();
}

float ATask_PlayerState::GetMaxHealth() const
{
    return AttributeSet->GetMaxHealth();
}
