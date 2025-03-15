

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
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(GetRootComponent());
	Mesh1P->bCastDynamicShadow = true;
	Mesh1P->CastShadow = true;

	BoomArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("BoomArm"));
	FName SocketName = TEXT("HatSocket");
	BoomArm->SetupAttachment(Mesh1P, SocketName);
	BoomArm->TargetArmLength = -20.0f;

	CharacterCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CharacterCameraComponent"));
	CharacterCameraComponent->SetupAttachment(BoomArm);
	CharacterCameraComponent->bUsePawnControlRotation = true;
}

void ATask_PlayableCharacher::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}


void ATask_PlayableCharacher::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocallyControlled())
	{
		SetActorTickEnabled(true);
	}

	else
	{
		SetActorTickEnabled(false);
	}

	if (ATask_PlayerControllerBase* playerController = Cast<ATask_PlayerControllerBase>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
			//GEngine->AddOnScreenDebugMessage(5, -1, FColor::Red, FString::Printf(TEXT("Added subsystem")), 1);
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

void ATask_PlayableCharacher::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UTask_EnhancedInputComponent* SInputComponent = Cast<UTask_EnhancedInputComponent>(PlayerInputComponent);
	check(SInputComponent);

	const FEGameplayTags& GameplayTags = FEGameplayTags::Get();
	TArray<uint32> BindHandles;

	SInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::InputAbilityInputTagPressed, &ThisClass::InputAbilityInputTagReleased, BindHandles);
	SInputComponent->BindNativeAction(InputConfig, GameplayTags.Task_Input_Move, ETriggerEvent::Triggered, this, &ThisClass::Move);
	SInputComponent->BindNativeAction(InputConfig, GameplayTags.Task_Input_Look, ETriggerEvent::Triggered, this, &ThisClass::Look);
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


void ATask_PlayableCharacher::Move(const FInputActionValue& Value)
{
	const FVector2D DirectionValue = Value.Get<FVector2D>();
	if (GetController())
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);

		const FVector ForwardVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(ForwardVector, DirectionValue.Y);

		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(RightDirection, DirectionValue.X);

		//GEngine->AddOnScreenDebugMessage(5, -1, FColor::Red, FString::Printf(TEXT("Move Pressed")), 1);
	}
}

void ATask_PlayableCharacher::Look(const FInputActionValue& Value)
{
	const FVector2D LookValue = Value.Get<FVector2D>();

	if (GetController())
	{
		if (LookValue.X != 0.0f)
		{
			AddControllerYawInput(LookValue.X);
		}

		if (LookValue.Y != 0.0f)
		{
			AddControllerPitchInput(-LookValue.Y);
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