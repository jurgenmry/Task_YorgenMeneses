// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Task_BaseCharacher.h"

// Sets default values
ATask_BaseCharacher::ATask_BaseCharacher()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

UAbilitySystemComponent* ATask_BaseCharacher::GetAbilitySystemComponent() const
{
	return nullptr;
}

// Called when the game starts or when spawned
void ATask_BaseCharacher::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATask_BaseCharacher::InitAbilityActorInfo()
{
}

void ATask_BaseCharacher::InitializeAttributes()
{
}

void ATask_BaseCharacher::AddCharacterAbilities()
{
}

void ATask_BaseCharacher::RemoveCharacterAbilities()
{
}

void ATask_BaseCharacher::RemoveAllGameplayEffects()
{
}

void ATask_BaseCharacher::ApplyGEToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level)
{
}



