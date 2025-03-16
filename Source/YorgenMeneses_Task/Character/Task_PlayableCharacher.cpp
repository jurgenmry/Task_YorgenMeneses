

#include "Character/Task_PlayableCharacher.h"

//AbilitySystem
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "GameplayTagsManager.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayEffectTypes.h"

#include "AbilitySystem/Task_AbilitySet.h"
#include "AbilitySystem/Task_AbilitySystemComponent.h"
#include "AbilitySystem/Task_AttributeSet.h"

//Multiplayer
#include "Net/UnrealNetwork.h"

//Components
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"

//Input
#include "EnhancedInputSubsystems.h"
#include "Input/Task_EnhancedInputComponent.h"
#include "Task_GameplayTags.h"
#include "Input/Task_InputConfig.h"

//Others
#include "Player/Task_PlayerControllerBase.h"
#include "Player/Task_PlayerState.h"

ATask_PlayableCharacher::ATask_PlayableCharacher()
	:BoxCollisionSize(FVector::ZeroVector)
	, LinearDamping(3.0f)
	, AngularDamping(5.0f)
	, SuspensionLength(20.0f)
	, SuspensionForce(8000.0f)
	, SteeringForce(25000.0f)
	, JumpForce(300.0f)
	, PlayerAcceleration(100.0f)
	, MaxBoardSpeed(1000.0f)
	, bMoving(false)
	, AccelerationRate(50.0f)
	, LessBoardFlipChance(0.5f)
	
{
	bReplicates = true;
	SetActorTickEnabled(true);

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	SetRootComponent(BoxCollision);
	BoxCollisionSize = FVector(60.f, 15.f, 3.f);
	BoxCollision->SetBoxExtent(BoxCollisionSize);
	BoxCollision->SetSimulatePhysics(true);
	BoxCollision->SetEnableGravity(true);
	BoxCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BoxCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);

	BoomArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("BoomArm"));
	BoomArm->SetupAttachment(GetRootComponent());
	BoomArm->TargetArmLength = 300.0f;

	SkateBoardMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SkateBoardMesh"));
	SkateBoardMesh->SetupAttachment(BoxCollision);

	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(false);
	Mesh1P->SetupAttachment(GetRootComponent());
	Mesh1P->bCastDynamicShadow = true;
	Mesh1P->CastShadow = true;

	CharacterCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CharacterCameraComponent"));
	CharacterCameraComponent->SetupAttachment(BoomArm);
	CharacterCameraComponent->bUsePawnControlRotation = true;

	FL_Wheel = CreateDefaultSubobject<USceneComponent>(TEXT("FL_Wheel"));
	FR_Wheel = CreateDefaultSubobject<USceneComponent>(TEXT("FR_Wheel"));
	BL_Wheel = CreateDefaultSubobject<USceneComponent>(TEXT("BL_Wheel"));
	BR_Wheel = CreateDefaultSubobject<USceneComponent>(TEXT("BR_Wheel"));

	FL_Wheel->SetupAttachment(SkateBoardMesh);
	FR_Wheel->SetupAttachment(SkateBoardMesh);
	BL_Wheel->SetupAttachment(SkateBoardMesh);
	BR_Wheel->SetupAttachment(SkateBoardMesh);

	Tires.Add(FL_Wheel);
	Tires.Add(FR_Wheel);
	Tires.Add(BL_Wheel);
	Tires.Add(BR_Wheel);

	BoxCollision->SetAngularDamping(AngularDamping);
	BoxCollision->SetLinearDamping(LinearDamping);
}

void ATask_PlayableCharacher::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void ATask_PlayableCharacher::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	UpdateSpeed(DeltaSeconds);

	for (USceneComponent* Tire : Tires)
	{
		BoardSuspention(Tire);
	}
}



void ATask_PlayableCharacher::BeginPlay()
{
	Super::BeginPlay();

	InitialRotation = GetActorRotation();

	ATask_PlayerControllerBase* PlayerController = Cast<ATask_PlayerControllerBase>(GetLocalViewingPlayerController());
	EnableInput(PlayerController);
	ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();
	if (LocalPlayer)
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void ATask_PlayableCharacher::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UTask_EnhancedInputComponent* SInputComponent = Cast<UTask_EnhancedInputComponent>(PlayerInputComponent);
	check(SInputComponent);

	const FEGameplayTags& GameplayTags = FEGameplayTags::Get();
	TArray<uint32> BindHandles;

	SInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::InputAbilityInputTagPressed, &ThisClass::InputAbilityInputTagReleased, BindHandles);

	SInputComponent->BindNativeAction(InputConfig, GameplayTags.Task_Native_Input_LookUp, ETriggerEvent::Triggered, this, &ThisClass::LookUp);
	SInputComponent->BindNativeAction(InputConfig, GameplayTags.Task_Native_Input_Turn, ETriggerEvent::Triggered, this, &ThisClass::Turn);

	SInputComponent->BindNativeAction(InputConfig, GameplayTags.Task_Native_Input_Jump, ETriggerEvent::Triggered, this, &ThisClass::BoardJump);

	SInputComponent->BindNativeAction(InputConfig, GameplayTags.Task_Native_Input_MoveForward, ETriggerEvent::Triggered, this, &ThisClass::MoveForward);
	SInputComponent->BindNativeAction(InputConfig, GameplayTags.Task_Native_Input_MoveForward, ETriggerEvent::Completed, this, &ThisClass::MoveForward);

	SInputComponent->BindNativeAction(InputConfig, GameplayTags.Task_Native_Input_MoveRight, ETriggerEvent::Triggered, this, &ThisClass::MoveRight);
	SInputComponent->BindNativeAction(InputConfig, GameplayTags.Task_Native_Input_MoveRight, ETriggerEvent::Completed, this, &ThisClass::MoveRight);

	SInputComponent->BindNativeAction(InputConfig, GameplayTags.Task_Native_Input_Reset, ETriggerEvent::Triggered, this, &ThisClass::ResetCharacter);

}

void ATask_PlayableCharacher::LookUp(const FInputActionValue& Value)
{
	// get the flat angle value for the input 
	float LookValue = Value.Get<float>();
	AddControllerPitchInput(-LookValue);
}

void ATask_PlayableCharacher::Turn(const FInputActionValue& Value)
{
	float TurnValue = Value.Get<float>();
	AddControllerYawInput(TurnValue);
}

void ATask_PlayableCharacher::MoveForward(const FInputActionValue& Value)
{
	const float MovementInput = Value.Get<float>();

	// If input is nonzero, mark the board as moving and require a speed update.
	if (FMath::Abs(MovementInput) > KINDA_SMALL_NUMBER)
	{
		bMoving = true;
		bNeedsSpeedUpdate = true;
	}
	else
	{
		// If no input is held, the board should decelerate.
		bMoving = false;
	}
}

void ATask_PlayableCharacher::MoveRight(const FInputActionValue& Value)
{
	const float MovementInput = Value.Get<float>();
	if (CurrentBoardSpeed > 0.0f && FMath::Abs(MovementInput) > KINDA_SMALL_NUMBER)
	{
		// Apply torque around the Z-axis (up axis) to steer.
		FVector Torque = FVector(0.0f, 0.0f, MovementInput * SteeringForce);
		BoxCollision->AddTorqueInRadians(Torque);
	}
}

void  ATask_PlayableCharacher::BoardJump(const FInputActionValue& Value)
{
	// Only jump if all tires are on the ground.
	if (AllTiresGrounded())
	{
		// Play the jump animation on the character mesh, if one is set.
		if (Mesh1P && JumpAnimation)
		{
			Mesh1P->PlayAnimation(JumpAnimation, false);
		}

		// Apply an upward impulse from the bottom of the board.
		FVector JumpImpulse = FVector(0.0f, 0.0f, JumpForce);
		if (BoxCollision)
		{
			BoxCollision->AddImpulse(JumpImpulse);
		}

		// Random chance to stabilize angular velocity (reduce flipping).
		// For example, 50% chance to damp angular rotation.

		//Bellow: Not sure if its working .. Need more investigation.
		float RandomChance = FMath::FRand();  
		if (RandomChance < LessBoardFlipChance)  
		{
			// Print a debug message to the screen.
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Stabilizing jump angular velocity."));
			}

			// Get current angular velocity (in radians per second).
			FVector CurrentAngularVelocity = BoxCollision->GetPhysicsAngularVelocityInRadians();
			// Reduce the angular velocity by, say, 80%.
			FVector NewAngularVelocity = CurrentAngularVelocity * 0.1f;
			BoxCollision->SetPhysicsAngularVelocityInRadians(NewAngularVelocity, false);
		}
	}
}

void ATask_PlayableCharacher::ResetCharacter(const FInputActionValue& Value)
{
	FVector CurrentLocation = GetActorLocation();
	SetActorLocationAndRotation(CurrentLocation, InitialRotation);

	// Clear physics velocities.
	if (BoxCollision)
	{
		BoxCollision->SetPhysicsLinearVelocity(FVector::ZeroVector);
		BoxCollision->SetPhysicsAngularVelocityInRadians(FVector::ZeroVector);
	}

	// Reset movement state variables.
	CurrentBoardSpeed = 0.0f;
	bMoving = false;
	bNeedsSpeedUpdate = false;

	// Debug message to confirm reset.
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("Board reset to original orientation."));
	}
}

void ATask_PlayableCharacher::BoardSuspention(USceneComponent* Tire)
{
	if (!Tire) return;

	// Get the starting location of the tire and define the end point for the trace.
	FVector Start = Tire->GetComponentLocation();
	FVector End = Start + FVector(0.0f, 0.0f, -SuspensionLength);

	FHitResult HitResult;
	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(this); // Ignore the vehicle itself

	// Perform the line trace.
	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, TraceParams);

	if (HitResult.bBlockingHit)
	{
		// Calculate the hit distance.
		float HitDistance = HitResult.Distance;

		// Normalize the hit distance in the range [0, SuspensionLength] (0 when fully compressed, 1 when no compression)
		float NormalizedHitDistance = 1.0f - FMath::GetMappedRangeValueClamped(
			FVector2D(0.0f, SuspensionLength),
			FVector2D(0.0f, 1.0f),
			HitDistance
		);

		// stiffer response: force ramps up quickly with compression.
		float EffectiveCompression = NormalizedHitDistance * NormalizedHitDistance;

		FVector UnitDirection = (HitResult.TraceEnd - HitResult.TraceStart).GetSafeNormal();
		FVector SuspensionForces = (-UnitDirection * EffectiveCompression) * SuspensionForce;

		if (BoxCollision)
		{
			BoxCollision->AddForceAtLocation(SuspensionForces, Tire->GetComponentLocation());
		}

		// Debug line visualization (green line shows the impact point).
		DrawDebugLine(GetWorld(), Start, HitResult.ImpactPoint, FColor::Green, false, 10.0f, 0, 2.0f);
	}
	else
	{
		// Debug line visualization (red line shows the full trace range when no hit is detected).
		DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 1.0f, 0, 2.0f);
	}
}

bool ATask_PlayableCharacher::AllTiresGrounded()
{
	// Iterate over each tire
	for (USceneComponent* Tire : Tires)
	{
		if (!Tire) continue;

		// Set up the line trace starting at the tire and going downward.
		FVector Start = Tire->GetComponentLocation();
		FVector End = Start + FVector(0.0f, 0.0f, -SuspensionLength);

		FHitResult HitResult;
		FCollisionQueryParams TraceParams;
		TraceParams.AddIgnoredActor(this);

		bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, TraceParams);

		// If one tire does not register a blocking hit, then we consider the board not fully grounded.
		if (!bHit || !HitResult.bBlockingHit)
		{
			return false;
		}
	}
	return true;
}

void ATask_PlayableCharacher::UpdateSpeed(float DeltaTime)
{
	// Update speed only if necessary.
	if (HasAuthority() && bNeedsSpeedUpdate)
	{
		float TargetSpeed = bMoving ? MaxBoardSpeed : 0.0f;

		// Accelerate if below target speed.
		if (CurrentBoardSpeed < TargetSpeed)
		{
			CurrentBoardSpeed = FMath::Min(CurrentBoardSpeed + AccelerationRate * DeltaTime, TargetSpeed);
		}
		// Decelerate if above target speed.
		else if (CurrentBoardSpeed > TargetSpeed)
		{
			CurrentBoardSpeed = FMath::Max(CurrentBoardSpeed - AccelerationRate * DeltaTime, TargetSpeed);
		}

		// Ensure speed doesn't go negative.
		CurrentBoardSpeed = FMath::Max(CurrentBoardSpeed, 0.0f);

		// If the board is moving, update its velocity.
		if (CurrentBoardSpeed > 0.0f)
		{
			// Calculate the desired horizontal velocity in the forward direction.
			FVector ForwardVelocity = GetActorForwardVector() * CurrentBoardSpeed;

			// Get the current velocity
			FVector CurrentVelocity = BoxCollision->GetPhysicsLinearVelocity();
			ForwardVelocity.Z = CurrentVelocity.Z;

			// Update the physics linear velocity.
			BoxCollision->SetPhysicsLinearVelocity(ForwardVelocity);
		}

		// Stop updating if we've reached our speed target.
		if (FMath::IsNearlyEqual(CurrentBoardSpeed, TargetSpeed, 0.1f))
		{
			if (CurrentBoardSpeed == 0.0f)
			{
				bNeedsSpeedUpdate = false;
			}
		}
	}
}

void ATask_PlayableCharacher::InitAbilityActorInfo()
{
	ATask_PlayerState* MainPlayerState = GetPlayerState<ATask_PlayerState>();
	if (MainPlayerState->GetAbilitySystemComponent())
	{
		MainPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(MainPlayerState, this);
		AbilitySystemComponent = Cast<UTask_AbilitySystemComponent>(MainPlayerState->GetAbilitySystemComponent());
		if (AbilitySystemComponent->IsValidLowLevel())
		{
			AbilitySystemComponent->AbilityActorInfoSet();
			AttributeSet = MainPlayerState->GetAttributeSetBase();

			InitializeAttributes();

			/*if (AbilitySet)
			{
				AbilitySet->GiveToAbilitySystem(AbilitySystemComponent.Get(), nullptr, this);
			}
			*/
			//BindCallbacksToDependencies();
		}
	}
}

void ATask_PlayableCharacher::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	InitAbilityActorInfo();
}

void ATask_PlayableCharacher::PossessedBy(AController* NewController)
{
	//for the client 
	Super::PossessedBy(NewController);
	ATask_PlayerControllerBase* PC = Cast<ATask_PlayerControllerBase>(NewController);
	ATask_PlayerState* PS = Cast<ATask_PlayerState>(GetPlayerState());
	if (PS)
	{
		AbilitySystemComponent = Cast<UTask_AbilitySystemComponent>(PS->GetAbilitySystemComponent());
		AbilitySystemComponent->InitAbilityActorInfo(PS, this);
		InitAbilityActorInfo();

		if (AbilitySet)
		{
			AbilitySet->GiveToAbilitySystem(AbilitySystemComponent.Get(), nullptr, this);
		}
	}
	if (PC)
	{
		ULocalPlayer* LocalPlayer = PC->GetLocalPlayer();
		if (LocalPlayer)
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer))
			{
				Subsystem->AddMappingContext(DefaultMappingContext, 0);
			}
		}
	}
}

void ATask_PlayableCharacher::InputAbilityInputTagPressed(FGameplayTag InputTag)
{
	if (AbilitySystemComponent->IsValidLowLevel())
	{
		AbilitySystemComponent->AbilityInputTagPressed(InputTag);
	}
}

void ATask_PlayableCharacher::InputAbilityInputTagReleased(FGameplayTag InputTag)
{
	if (AbilitySystemComponent->IsValidLowLevel())
	{
		AbilitySystemComponent->AbilityInputTagReleased(InputTag);
	}
}