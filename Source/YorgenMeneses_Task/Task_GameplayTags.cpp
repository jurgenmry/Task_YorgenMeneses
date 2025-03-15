
#include "Task_GameplayTags.h"
#include "GameplayTagsManager.h"

FEGameplayTags FEGameplayTags::GameplayTags;

void FEGameplayTags::InitializeNativeTags()
{
	UGameplayTagsManager& Manager = UGameplayTagsManager::Get();

	GameplayTags.AddAllTags(Manager);

	Manager.DoneAddingNativeTags();
}

void FEGameplayTags::AddAllTags(UGameplayTagsManager& Manager)
{
	AddTag(Task_Input_Move, "Task.Input.Move", "Move input.");
	AddTag(Task_Input_Look, "Task.Input.Look", "Look input.");

	//AddTag(Ability_Input_OnInputTriggered, "Ability.Input.OnInputTriggered", "Test ability input.");
	//AddTag(Ability_Input_WhileInputActive, "Ability.Input.WhileInputActive", "Test ability input.");
}

void FEGameplayTags::AddTag(FGameplayTag& OutTag, const ANSICHAR* TagName, const ANSICHAR* TagComment)
{
	OutTag = UGameplayTagsManager::Get().AddNativeGameplayTag(FName(TagName), FString(TEXT("(Native) ")) + FString(TagComment));
}