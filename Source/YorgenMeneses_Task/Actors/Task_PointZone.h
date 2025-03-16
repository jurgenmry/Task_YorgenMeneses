

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Task_PointZone.generated.h"


class UBoxComponent;
class UStaticMeshComponent;

UCLASS()
class YORGENMENESES_TASK_API ATask_PointZone : public AActor
{

    GENERATED_BODY()

public:

    ATask_PointZone();

protected:

    virtual void BeginPlay() override;


    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
            UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
            bool bFromSweep, const FHitResult& SweepResult);
    
    UFUNCTION()
    void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, 
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp, 
        int32 OtherBodyIndex);

public:
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UBoxComponent* CollisionBox;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* Mesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Points")
    int32 MinPoints = 10;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Points")
    int32 MaxPoints = 50;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Points")
    float AboveMeshTolerance = 10.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Points")
    float UprightThresholdDegrees = 30.0f;

};
