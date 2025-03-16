

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
class UStaticMeshComponent;
class UBoxComponent;
class USpringArmComponent;
class UCameraComponent;
class USceneComponent;
class UTask_AbilitySet;
class UTask_InputConfig;

UCLASS()
class YORGENMENESES_TASK_API ATask_PlayableCharacher : public ATask_BaseCharacher
{
	GENERATED_BODY()

public:

	ATask_PlayableCharacher();

	//Variables:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Settings")
	FVector BoxCollisionSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Settings")
	float LinearDamping;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Settings")
	float AngularDamping;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Settings")
	float SuspensionLength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Settings")
	float SuspensionForce;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Settings")
	float SteeringForce;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Settings")
	float JumpForce;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Settings")
	float PlayerAcceleration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Settings")
	float MaxBoardSpeed;

	UPROPERTY(BlueprintReadWrite, Category = "Player Settings");
	bool bMoving;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Settings", meta = (ClampMin = "10.0", ClampMax = "100.0"))
	float AccelerationRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Player Settings")
	float CurrentBoardSpeed;

	UPROPERTY(BlueprintReadWrite, Category = "Player Settings")
	bool bNeedsSpeedUpdate;

	UPROPERTY(EditAnywhere, Category = "Player Settings", meta = (ClampMin = "0.0", ClampMax = "1"))
	float LessBoardFlipChance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Settings")
	UAnimMontage* JumpAnimation;

	FRotator InitialRotation; // for the reset bug

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Components")
	TArray<USceneComponent*>Tires;

	//components

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* BoxCollision;
	FORCEINLINE UBoxComponent* GetBoxCollision() const { return BoxCollision; }

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Mesh1P;
	FORCEINLINE USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* CharacterCameraComponent;
	FORCEINLINE UCameraComponent* GetCameraComponent() const { return CharacterCameraComponent; }

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USpringArmComponent* BoomArm;
	FORCEINLINE USpringArmComponent* GetSpringArm() const { return BoomArm; }

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* SkateBoardMesh;
	FORCEINLINE UStaticMeshComponent* GetSkateBoard() const { return SkateBoardMesh; }

	//Scene componenets for wheels
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* FL_Wheel;
	FORCEINLINE USceneComponent* GetFLWheel() const { return FL_Wheel; }

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* FR_Wheel;
	FORCEINLINE USceneComponent* GetFR_Wheel() const { return FR_Wheel; }
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* BL_Wheel;
	FORCEINLINE USceneComponent* GetBL_Wheel() const { return BL_Wheel; }

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* BR_Wheel;
	FORCEINLINE USceneComponent* GetBR_Wheel() const { return BR_Wheel; }


	//Functions
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PossessedBy(AController* NewController) override;


	void InputAbilityInputTagPressed(FGameplayTag InputTag);
	void InputAbilityInputTagReleased(FGameplayTag InputTag);

	virtual void InitAbilityActorInfo() override;

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	void LookUp(const FInputActionValue& Value);
	void Turn(const FInputActionValue& Value);
	void MoveForward(const FInputActionValue& Value);
	void MoveRight(const FInputActionValue& Value);
	void BoardJump(const FInputActionValue& Value);
	void ResetCharacter(const FInputActionValue& Value);

	void BoardSuspention(USceneComponent* Tire);
	bool AllTiresGrounded();
	void UpdateSpeed(float DeltaTime);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "InputSystem|Input")
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "InputSystem|Abilities")
	UTask_AbilitySet* AbilitySet;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "InputSystem|Input")
	UTask_InputConfig* InputConfig;

	// Client only
	virtual void OnRep_PlayerState() override;
	
};
