// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
// Including the Game Character Type header to access the Enumeration ECharacterState
#include "CharacterTypes.h"
#include "GameProject1AnimInstance.generated.h"

class GameProjectCharacter;
/**
 * 
 */
UCLASS()
class GAMEPROJECT_1_API UGameProject1AnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	// C++ Equivalent of Event Blueprint Initialize Animation Event Node
	virtual void NativeInitializeAnimation() override;
	// C++ Equivalent of Event Blueprint Update Animation Event Node
	virtual void NativeUpdateAnimation(float DeltaTime) override;
	
	UPROPERTY(BlueprintReadOnly);
	TObjectPtr<class AGameProjectCharacter> GameProjectCharacter;

	UPROPERTY(BlueprintReadOnly, Category = Movement);
	TObjectPtr<class UCharacterMovementComponent> GameProjectCharacterMovementComponent;

	UPROPERTY(BlueprintReadOnly, Category = Movement);
	float GroundSpeed;

	UPROPERTY(BlueprintReadOnly, Category = Movement);
	bool IsFalling;

	UPROPERTY(BlueprintReadOnly, Category = "Movement | Character State");
	ECharacterState CharacterState;
};
