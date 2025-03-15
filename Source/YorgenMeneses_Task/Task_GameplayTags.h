
#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

class UGameplayTagsManager;


struct YORGENMENESES_TASK_API FEGameplayTags
{

public:
	static const FEGameplayTags& Get() { return GameplayTags; }
	static void InitializeNativeTags();

	
	FGameplayTag Task_Native_Input_LookUp;
	FGameplayTag Task_Native_Input_MoveRight;
	FGameplayTag Task_Native_Input_MoveForward;
	FGameplayTag Task_Native_Input_Turn;
	FGameplayTag Task_Native_Input_Camera_Zoom;

	FGameplayTag Ability_Input_WhileInputActive;
	FGameplayTag Ability_Input_OnInputTriggered;

protected:

	void AddAllTags(UGameplayTagsManager& Manager);
	void AddTag(FGameplayTag& OutTag, const ANSICHAR* TagName, const ANSICHAR* TagComment);

private:

	static FEGameplayTags GameplayTags;
};