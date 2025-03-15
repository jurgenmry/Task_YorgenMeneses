


#include "Task_AssetManager.h"
#include "Task_GameplayTags.h"
#include "AbilitySystemGlobals.h"

UTask_AssetManager::UTask_AssetManager()
{
}

UTask_AssetManager& UTask_AssetManager::Get()
{
	check(GEngine)
		if (UTask_AssetManager* Singleton = Cast<UTask_AssetManager>(GEngine->AssetManager))
		{
			return *Singleton;
		}

	UE_LOG(LogTemp, Fatal, TEXT("Invalid AssetManagerClassName in DefaultEngine.ini. It must be set to UTTAssetManager!"))

		return *NewObject<UTask_AssetManager>();
}

void UTask_AssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
	FEGameplayTags::InitializeNativeTags();
	UAbilitySystemGlobals::Get().InitGlobalData();
}
