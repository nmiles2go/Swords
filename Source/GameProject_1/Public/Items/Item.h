// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

class USphereComponent;

enum class EItemState : uint8
{
	EIS_Hovering,
	EIS_Equipped
};

UCLASS()
class GAMEPROJECT_1_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sine Parameters")
	int Amplitude = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sine Parameters")
	int TimeConstant = 5.0f; 

	UPROPERTY(EditAnywhere, BlueprintReadOnly);
	TObjectPtr<UStaticMeshComponent> ItemMesh;

	UFUNCTION(BlueprintPure)
	float TransformedSin();
	
	UFUNCTION(BlueprintPure)
	float TransformedCos();

	template <typename T>
	T Avg(T First, T Second);

	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, 
								 AActor* OtherActor, 
								 UPrimitiveComponent* OtherComp, 
								 int32 OtherBodyIndex, 
								 bool bFromSweep, 
								 const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnSphereOverlapEnd(UPrimitiveComponent* OverlappedComponent, 
									AActor* OtherActor, 
									UPrimitiveComponent* OtherComp, 
									int32 OtherBodyIndex);
	
	
	EItemState ItemState = EItemState::EIS_Hovering;

	UPROPERTY(EditAnywhere);
	TObjectPtr<USphereComponent> Sphere;

private: 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"));
	float RunningTime;


	FRotator Rotation = FRotator(0.0f, 35.0f, 0.0f);

};

template<typename T>
inline T AItem::Avg(T First, T Second)
{
	return (First + Second)/2;
}
