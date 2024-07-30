// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Item.h"
#include"DrawDebugHelpers.h"
#include "Components/SphereComponent.h"
#include "GameProject_1/DebugMacros.h"
#include "Characters/GameProjectCharacter.h"

// Sets default values
AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComponent"));
	RootComponent = ItemMesh;
	
	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collision"));
	Sphere->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnSphereOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &AItem::OnSphereOverlapEnd);

}

float AItem::TransformedSin() {
	return Amplitude * FMath::Sin(RunningTime * TimeConstant) / 25.0;
}

float AItem::TransformedCos() {
	return Amplitude * FMath::Cos(RunningTime * TimeConstant);
}

void AItem::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, 
							AActor* OtherActor, 
							UPrimitiveComponent* OtherComp, 
							int32 OtherBodyIndex, 
							bool bFromSweep, 
							const FHitResult& SweepResult)
{
	AGameProjectCharacter* GameProjectCharacter = Cast<AGameProjectCharacter>(OtherActor);
	
	if (GameProjectCharacter) {
		GameProjectCharacter->SetOverlappingItem(this);
	}

}

void AItem::OnSphereOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	const FString OtherActorName = FString("Ending Overlap with: ") + OtherActor->GetName();

	AGameProjectCharacter* GameProjectCharacter = Cast<AGameProjectCharacter>(OtherActor);

	if (GameProjectCharacter) {
		GameProjectCharacter->SetOverlappingItem(nullptr);
	}
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RunningTime += DeltaTime;

	if (ItemState == EItemState::EIS_Hovering)
	{
		AddActorWorldOffset(FVector(0.f, 0.f, TransformedSin()));
		AddActorWorldRotation(Rotation * DeltaTime);
	}

	
}

