// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/GameProject1AnimInstance.h"
#include "Characters/GameProjectCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UGameProject1AnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	// TryGetPawnOwner() - Returns a Pawn 
	if (GameProjectCharacter = Cast<AGameProjectCharacter>(TryGetPawnOwner())) {
		GameProjectCharacterMovementComponent = GameProjectCharacter->GetCharacterMovement();
	}
}

void UGameProject1AnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (GameProjectCharacterMovementComponent) {
		GroundSpeed = UKismetMathLibrary::VSizeXY(GameProjectCharacterMovementComponent->Velocity);
		IsFalling = GameProjectCharacterMovementComponent->IsFalling();
		CharacterState = GameProjectCharacter->GetCharacterState();
	}

}

