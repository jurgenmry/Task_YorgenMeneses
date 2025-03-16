
#include "Actors/Task_PointZone.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Character/Task_PlayableCharacher.h"
#include "Player/Task_PlayerControllerBase.h"

ATask_PointZone::ATask_PointZone()
{
 	
    PrimaryActorTick.bCanEverTick = false;

    CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
    RootComponent = CollisionBox;
    CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    CollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    Mesh->SetupAttachment(RootComponent);

}

void ATask_PointZone::BeginPlay()
{
	Super::BeginPlay();

    // Bind the overlap end event.
    if (CollisionBox)
    {
        CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ATask_PointZone::OnOverlapBegin);
        CollisionBox->OnComponentEndOverlap.AddDynamic(this, &ATask_PointZone::OnOverlapEnd);
    }
	
}

void ATask_PointZone::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void ATask_PointZone::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (OtherActor && OtherActor != NULL && OtherActor != this)
    {
        ATask_PlayableCharacher* PlayerChar = Cast<ATask_PlayableCharacher>(OtherActor);
        if (PlayerChar)
        {
        
            FVector RelativeLocation = PlayerChar->GetActorLocation() - Mesh->GetComponentLocation();
            float AboveValue = FVector::DotProduct(RelativeLocation, Mesh->GetUpVector());
            if (AboveValue < AboveMeshTolerance)
            {
                return;
            }

            float UprightDot = FVector::DotProduct(PlayerChar->GetActorUpVector(), FVector::UpVector);
            // Calculate the cosine threshold from the allowed tilt angle.
            float UprightCosineThreshold = FMath::Cos(FMath::DegreesToRadians(UprightThresholdDegrees));
            if (UprightDot < UprightCosineThreshold)
            {
                // Player is too tilted or flipped.
                FString DebugMsg = FString::Printf(TEXT("Player to tilted!"));
                GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, DebugMsg);
                return;
            }

            int32 AwardedPoints = FMath::RandRange(MinPoints, MaxPoints);
            
            PlayerChar->AddPointsToHUD(AwardedPoints);
            /*
            if (GEngine)
            {
                FString DebugMsg = FString::Printf(TEXT("Awarded %d points!"), AwardedPoints);
                GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, DebugMsg);
            }
            */
        }
     } 
}



