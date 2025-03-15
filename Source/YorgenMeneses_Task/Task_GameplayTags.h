
#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

class UGameplayTagsManager;


struct YORGENMENESES_TASK_API FEGameplayTags
{

public:
	static const FEGameplayTags& Get() { return GameplayTags; }
	static void InitializeNativeTags();

	FGameplayTag Task_Native_Input_Move;
	FGameplayTag Task_Native_Input_Look;

	FGameplayTag Ability_Input_WhileInputActive;
	FGameplayTag Ability_Input_OnInputTriggered;

protected:

	void AddAllTags(UGameplayTagsManager& Manager);
	void AddTag(FGameplayTag& OutTag, const ANSICHAR* TagName, const ANSICHAR* TagComment);

private:

	static FEGameplayTags GameplayTags;
};