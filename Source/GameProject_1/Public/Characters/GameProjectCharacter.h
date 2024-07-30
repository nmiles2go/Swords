// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "CharacterTypes.h"



// this has to be at the end
#include "GameProjectCharacter.generated.h"

class UInputMappingContext;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;
class UGroomComponent;
class AItem;
class UAnimMontage;
class AWeapon;



UCLASS()
class GAMEPROJECT_1_API AGameProjectCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGameProjectCharacter();

	// Override ACharacter Jump() function
	virtual void Jump() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	void SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input);
	UInputMappingContext* EchoMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input);
	UInputAction* EchoMovementAction;

	// Reference to the characters's Input Action. 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* EchoLookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* EchoJumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* EchoEKeyPressedAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> EchoAttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> EchoDodgeAction;
	
	// Callbacks to Input
	void EKeyPressed();
	void Attack();
	void Dodge();
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

	/**
	* Play montage functions
	*/
	void PlayAttackMontage();
	void PlayEquipUnequipMontage(const FName& SectionName);

	// Functions for registering when our attack ends and if we can attack.
	UFUNCTION(BlueprintCallable)
	void AttackEnd();
	bool CanAttack();
	bool CanDisarm();
	bool CanArm();

	UFUNCTION(BlueprintCallable)
	void Disarm();

	UFUNCTION(BlueprintCallable)
	void Arm();

	UFUNCTION(BlueprintCallable)
	void FinishEquipping();

	

private:

	UPROPERTY(VisibleAnywhere);
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere);
	TObjectPtr<UCameraComponent> ViewCamera;

	UPROPERTY(VisibleAnywhere, Category = "Hair");
	TObjectPtr<UGroomComponent> EchoHair;

	UPROPERTY(VisibleAnywhere, Category = "Hair");
	TObjectPtr<UGroomComponent> EchoEyebrow;

	UPROPERTY(VisibleInstanceOnly);
	TObjectPtr<AItem> OverlappingItem;

	ECharacterState CharacterState = ECharacterState::ECS_Unequipped;
	
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"));
	EActionState ActionState = EActionState::EAS_Unoccupied;

	UPROPERTY(VisibleAnywhere, Category = "Weapon");
	TObjectPtr<AWeapon> EquippedWeapon;

	/**
	* Animation montages 
	*/
	UPROPERTY(EditDefaultsOnly, Category = "Montages");
	TObjectPtr<UAnimMontage> AttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Montages");
	TObjectPtr<UAnimMontage> EquipUnequipMontage;

public:

	FORCEINLINE void SetOverlappingItem(AItem* Item) { OverlappingItem = Item; }
	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; }
	
	
};
