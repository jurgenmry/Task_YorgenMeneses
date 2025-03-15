
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
	AddTag(Task_Native_Input_MoveForward, "Task.Native.Input.MoveForward", "Move input.");
	AddTag(Task_Native_Input_MoveRight, "Task.Native.Input.MoveRight", "Move input.");
	AddTag(Task_Native_Input_LookUp, "Task.Native.Input.LookUp", "Look input.");
	AddTag(Task_Native_Input_Turn, "Task.Native.Input.Turn", "Look input.");
	AddTag(Task_Native_Input_Camera_Zoom, "Task.Native.Input.CameraZoom", "Zoom input.");

	AddTag(Ability_Input_OnInputTriggered, "Ability.Input.OnInputTriggered", "Test ability input.");
	AddTag(Ability_Input_WhileInputActive, "Ability.Input.WhileInputActive", "Test ability input.");
}

void FEGameplayTags::AddTag(FGameplayTag& OutTag, const ANSICHAR* TagName, const ANSICHAR* TagComment)
{
	OutTag = UGameplayTagsManager::Get().AddNativeGameplayTag(FName(TagName), FString(TEXT("(Native) ")) + FString(TagComment));
}