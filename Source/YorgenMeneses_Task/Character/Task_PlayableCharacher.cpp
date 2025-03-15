

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

//Input
#include "EnhancedInputSubsystems.h"
#include "Input/Task_EnhancedInputComponent.h"
#include "Task_GameplayTags.h"
#include "Input/Task_InputConfig.h"

//Others
#include "Player/Task_PlayerControllerBase.h"
#include "Player/Task_PlayerState.h"

ATask_PlayableCharacher::ATask_PlayableCharacher()
{
	bReplicates = true;

	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(false);
	Mesh1P->SetupAttachment(GetRootComponent());
	Mesh1P->bCastDynamicShadow = true;
	Mesh1P->CastShadow = true;

	BoomArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("BoomArm"));
	BoomArm->SetupAttachment(GetRootComponent());
	BoomArm->TargetArmLength = 300.0f;

	SkateBoardMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SkateBoardMesh"));
	SkateBoardMesh->SetupAttachment(Mesh1P);

	CharacterCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CharacterCameraComponent"));
	CharacterCameraComponent->SetupAttachment(BoomArm);
	CharacterCameraComponent->bUsePawnControlRotation = true;
}

void ATask_PlayableCharacher::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void ATask_PlayableCharacher::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ATask_PlayableCharacher::BeginPlay()
{
	Super::BeginPlay();

	if (ATask_PlayerControllerBase* playerController = Cast<ATask_PlayerControllerBase>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
			GEngine->AddOnScreenDebugMessage(5, -1, FColor::Red, FString::Printf(TEXT("Added subsystem")), 1);
		}
	}

	if (IsLocallyControlled())
	{
		SetActorTickEnabled(true);
	}

	else
	{
		SetActorTickEnabled(false);
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
	SInputComponent->BindNativeAction(InputConfig, GameplayTags.Task_Native_Input_MoveForward, ETriggerEvent::Triggered, this, &ThisClass::MoveForward);
	SInputComponent->BindNativeAction(InputConfig, GameplayTags.Task_Native_Input_MoveRight, ETriggerEvent::Triggered, this, &ThisClass::MoveRight);
	SInputComponent->BindNativeAction(InputConfig, GameplayTags.Task_Native_Input_LookUp, ETriggerEvent::Triggered, this, &ThisClass::LookUp);
	SInputComponent->BindNativeAction(InputConfig, GameplayTags.Task_Native_Input_Turn, ETriggerEvent::Triggered, this, &ThisClass::Turn);
	SInputComponent->BindNativeAction(InputConfig, GameplayTags.Task_Native_Input_Camera_Zoom, ETriggerEvent::Triggered, this, &ThisClass::UpdateCameraZoom);
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
}


void ATask_PlayableCharacher::MoveForward(const FInputActionValue& Value)
{
}

void ATask_PlayableCharacher::MoveRight(const FInputActionValue& Value)
{
}

void ATask_PlayableCharacher::LookUp(const FInputActionValue& Value)
{
	float LookValue = Value.Get<float>();
	AddControllerPitchInput(-LookValue);
}

void ATask_PlayableCharacher::Turn(const FInputActionValue& Value)
{
	float TurnValue = Value.Get<float>();
	AddControllerYawInput(TurnValue);
}

void ATask_PlayableCharacher::UpdateCameraZoom(float DeltaTime)
{
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