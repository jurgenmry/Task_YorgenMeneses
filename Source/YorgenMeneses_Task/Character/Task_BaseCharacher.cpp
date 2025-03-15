// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Task_BaseCharacher.h"

// Sets default values
ATask_BaseCharacher::ATask_BaseCharacher()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATask_BaseCharacher::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATask_BaseCharacher::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATask_BaseCharacher::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

