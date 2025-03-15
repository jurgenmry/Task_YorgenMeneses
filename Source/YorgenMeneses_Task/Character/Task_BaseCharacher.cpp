
#include "Character/Task_BaseCharacher.h"

//Components
#include "Components/CapsuleComponent.h"

// System includes
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameplayTagsManager.h"

//custom includes
#include "AbilitySystem/Task_AbilitySystemComponent.h"
#include "AbilitySystem/Task_AttributeSet.h"


ATask_BaseCharacher::ATask_BaseCharacher()
{
	PrimaryActorTick.bCanEverTick = false;
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Overlap);
	bAlwaysRelevant = true;
}

void ATask_BaseCharacher::BeginPlay()
{
	Super::BeginPlay();

}

UAbilitySystemComponent* ATask_BaseCharacher::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent.Get();
}

void ATask_BaseCharacher::InitAbilityActorInfo(){}

void ATask_BaseCharacher::InitializeAttributes()
{
	ApplyGEToSelf(VitaltAttributes, 1.f);
}


void ATask_BaseCharacher::RemoveAllGameplayEffects()
{
	if (AbilitySystemComponent->IsValidLowLevel())
	{
		FGameplayEffectQuery Query;
		TArray<FActiveGameplayEffectHandle> ActiveEffects = AbilitySystemComponent->GetActiveEffects(Query);

		for (const FActiveGameplayEffectHandle& EffectHandle : ActiveEffects)
		{
			AbilitySystemComponent->RemoveActiveGameplayEffect(EffectHandle);
		}
	}
}

void ATask_BaseCharacher::ApplyGEToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level)
{
	if (!AbilitySystemComponent->IsValidLowLevel())
	{
		return;
	}

	if (!GameplayEffectClass)
	{
		UE_LOG(LogTemp, Error, TEXT("%s() Missing DefaultAttributes for %s. Please fill in the character's Blueprint."), *FString(__FUNCTION__), *GetName());
		return;
	}
	// Can run on Server and Client
	FGameplayEffectContextHandle EffectContext = GetAbilitySystemComponent()->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	const FGameplayEffectSpecHandle NewHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(GameplayEffectClass, Level, EffectContext);
	if (NewHandle.IsValid())
	{
		FActiveGameplayEffectHandle ActiveGEHandle = GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), GetAbilitySystemComponent());
	}
}



