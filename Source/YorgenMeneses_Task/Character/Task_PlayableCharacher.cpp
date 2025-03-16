

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
	, PlayerAcceleration(100.0f)
	
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
	
	if (BoxCollision)
	{
		// Reset the velocity slightly each tick to avoid infinite stacking forces
		FVector CurrentVelocity = BoxCollision->GetPhysicsLinearVelocity();
		BoxCollision->SetPhysicsLinearVelocity(FVector(CurrentVelocity.X, CurrentVelocity.Y, 0.0f));
	}

	for (USceneComponent* Tire : Tires)
	{
		CarSuspention(Tire);
	}
}



void ATask_PlayableCharacher::BeginPlay()
{
	Super::BeginPlay();

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

	SInputComponent->BindNativeAction(InputConfig, GameplayTags.Task_Native_Input_MoveForward, ETriggerEvent::Triggered, this, &ThisClass::MoveForward);
	SInputComponent->BindNativeAction(InputConfig, GameplayTags.Task_Native_Input_MoveForward, ETriggerEvent::Completed, this, &ThisClass::MoveForward);

	SInputComponent->BindNativeAction(InputConfig, GameplayTags.Task_Native_Input_MoveRight, ETriggerEvent::Triggered, this, &ThisClass::MoveRight);
	SInputComponent->BindNativeAction(InputConfig, GameplayTags.Task_Native_Input_MoveRight, ETriggerEvent::Completed, this, &ThisClass::MoveRight);

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
	// Get the movement input from the input action
	const float MovementInput = Value.Get<float>();
	if (MovementInput != 0.0f)
	{
		FVector ForwardDirection = GetActorForwardVector();
		// Iterate through all tire components
		for (USceneComponent* Tire : Tires)
		{
			if (Tire)
			{
				FVector TireLocation = Tire->GetComponentLocation();
				// Apply force at each tire position
				GetBoxCollision()->AddForceAtLocation(
					ForwardDirection * MovementInput * PlayerAcceleration * BoxCollision->GetMass(),
					TireLocation);
			}
		}
	}
}

void ATask_PlayableCharacher::MoveRight(const FInputActionValue& Value)
{
}

void ATask_PlayableCharacher::CarSuspention(USceneComponent* Tire)
{

	if (!Tire) return;

	// Get the starting location of the tire
	FVector Start = Tire->GetComponentLocation();
	FVector DirectionVector = Tire->GetUpVector()* FVector(0.0f, 0.0f, -SuspensionLength);
	FVector End = Start + DirectionVector; // Trace downward for suspension length

	// Line Trace Setup
	FHitResult HitResult;
	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(this); // Ignore the vehicle itself

	// Perform the line trace
	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, TraceParams);

	if (HitResult.bBlockingHit && HitResult.Distance < (SuspensionLength * 0.9f))
	{
		// Calculate how much the suspension is compressed
		float HitDistance = HitResult.Distance;

		// Normalize the distance in range [0, SuspensionLength] and subtract from 1
		float NormalizedHitDistance = 1.0f - FMath::GetMappedRangeValueClamped(
			FVector2D(0.0f, SuspensionLength),
			FVector2D(0.0f, 1.0f),
			HitDistance
		);

		// Get the direction from the hit point to the wheel (unit direction vector)
		FVector UnitDirection = (HitResult.TraceEnd - HitResult.TraceStart).GetSafeNormal();

		// Calculate suspension force
		FVector SuspensionForces = (-UnitDirection * NormalizedHitDistance) * SuspensionForce;

		if (BoxCollision)
		{
			BoxCollision->AddForceAtLocation(SuspensionForces, Tire->GetComponentLocation());
		}

		// Debug Visualization
		DrawDebugLine(GetWorld(), Start, HitResult.ImpactPoint, FColor::Green, false, 10.0f, 0, 2.0f);
	}
	else
	{
		// If no hit, draw debug red line to show the full trace range
		DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 1.0f, 0, 2.0f);
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