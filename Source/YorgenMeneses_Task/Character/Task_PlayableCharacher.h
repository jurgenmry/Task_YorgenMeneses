

#pragma once

#include "CoreMinimal.h"
#include "Character/Task_BaseCharacher.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "AbilitySystemComponent.h"
#include "InputActionValue.h"
#include "Task_PlayableCharacher.generated.h"


class UInputMappingContext;
class USkeletalMeshComponent;
class USpringArmComponent;
class UCameraComponent;
class UTask_AbilitySet;
class UTask_InputConfig;

UCLASS()
class YORGENMENESES_TASK_API ATask_PlayableCharacher : public ATask_BaseCharacher
{
	GENERATED_BODY()

public:

	ATask_PlayableCharacher();

	//components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Mesh1P;
	FORCEINLINE USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* CharacterCameraComponent;
	FORCEINLINE UCameraComponent* GetCameraComponent() const { return CharacterCameraComponent; }

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USpringArmComponent* BoomArm;
	FORCEINLINE USpringArmComponent* GetSpringArm() const { return BoomArm; }


	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PossessedBy(AController* NewController) override;


	void InputAbilityInputTagPressed(FGameplayTag InputTag);
	void InputAbilityInputTagReleased(FGameplayTag InputTag);

	virtual void Move(const FInputActionValue& Value);
	virtual void Look(const FInputActionValue& Value);

	virtual void InitAbilityActorInfo() override;

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "InputSystem|Input")
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "InputSystem|Abilities")
	UTask_AbilitySet* AbilitySet;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "InputSystem|Input")
	UTask_InputConfig* InputConfig;

	// Client only
	virtual void OnRep_PlayerState() override;
	
};
