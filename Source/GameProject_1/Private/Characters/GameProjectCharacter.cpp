// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/GameProjectCharacter.h"

#include "Components/InputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

#include "GameFramework/SpringArmComponent.h"

#include "Camera/CameraComponent.h"

#include "GameFramework/CharacterMovementComponent.h"

#include "GroomComponent.h"

#include "Items/Item.h"

#include "Items/Weapons/Weapon.h"

#include "Animation/AnimMontage.h"

#include "Components/BoxComponent.h"

// Sets default values
AGameProjectCharacter::AGameProjectCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Orient the character rotation towards the movement of the character
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 400.0f, 0.0f);

	// Declares the spring arm component and attaches it to the root component.
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	SpringArm->SetupAttachment(GetRootComponent());
	// Sets the arm length to 300.f
	SpringArm->TargetArmLength = 300.0f;

	// Makes the Spring arm rotate with the Look() function
	SpringArm ->bUsePawnControlRotation = true;

	// Rotate the camera above Echo
	SpringArm->AddRelativeRotation(FRotator(-20.f, 0.f, 0.f));

	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	ViewCamera->SetupAttachment(SpringArm);

	// Stops the camera from rotating with the controller. Only the spring arm.
	ViewCamera->bUsePawnControlRotation = false;

	// Adding Hair and Eyebrows to Echo
	EchoHair = CreateDefaultSubobject<UGroomComponent>(TEXT("Hair of Echo"));
	EchoHair->SetupAttachment(GetMesh());
	EchoHair->AttachmentName = FString("head");

	EchoEyebrow = CreateDefaultSubobject<UGroomComponent>(TEXT("Eyebrow of Echo"));
	EchoEyebrow->SetupAttachment(GetMesh());
	EchoEyebrow->AttachmentName = FString("head");


}

void AGameProjectCharacter::SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled)
{
	if (EquippedWeapon && EquippedWeapon->GetWeaponBox())
	{
		EquippedWeapon->GetWeaponBox()->SetCollisionEnabled(CollisionEnabled);
		EquippedWeapon->IgnoreActors.Empty();
	}
}

void AGameProjectCharacter::Jump()
{
	Super::Jump();
}

void AGameProjectCharacter::EKeyPressed()
{
	// Not the best and optimized way to pick up. Cast can be avaoided here.
	if (AWeapon* OverlappingWeapon = Cast<AWeapon>(OverlappingItem)) {
		OverlappingWeapon->Equip(GetMesh(), FName("hand_r_socket"));
		CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;

		// We set the overlapping item to null. And the reason  next time we press the key after 
		// we've already equipped a weapon, we don't
		// want that overlapping item to still store the address of the weapon we just picked up.
		// Otherwise, we'll try to do this whole process again.
		// We'll try to equip it again.
		// We'll try to set the character state again, but we've already picked up that weapon.
		// We only want to do this if we overlap a new item.
		OverlappingItem = nullptr;
		EquippedWeapon = OverlappingWeapon;
		
	}
	else {

		UE_LOG(LogTemp, Warning, TEXT("The boolean value CanDisArm is %s"), (CanDisarm() ? TEXT("true") : TEXT("false")));

		if (CanDisarm()) {
			PlayEquipUnequipMontage(FName("Unequip"));
			CharacterState = ECharacterState::ECS_Unequipped;
			ActionState = EActionState::EAS_EquippingWeapon;
		}
		 
		else if (CanArm()) {
			PlayEquipUnequipMontage(FName("Equip"));
			CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
			ActionState = EActionState::EAS_EquippingWeapon;
		}

	}

}

void AGameProjectCharacter::Attack()
{
	if (CanAttack())
	{
		PlayAttackMontage();
		ActionState = EActionState::EAS_Attacking;
	}
	
}

void AGameProjectCharacter::Dodge()
{

}

// Called when the game starts or when spawned
void AGameProjectCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController())) {
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			Subsystem->AddMappingContext(EchoMappingContext, 0);
		}
	}
}

void AGameProjectCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();
	
	if (ActionState != EActionState::EAS_Unoccupied) return;

	if (Controller) {

		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);	

	
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(ForwardDirection, MovementVector.Y);

		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
	
}

void AGameProjectCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisValue = Value.Get<FVector2D>();
	if (Controller)
	{
		AddControllerYawInput(LookAxisValue.X);
		AddControllerPitchInput(LookAxisValue.Y);
	}
}

void AGameProjectCharacter::PlayAttackMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && AttackMontage)
	{
		AnimInstance->Montage_Play(AttackMontage);
		const int32 Selection = FMath::RandRange(0, 3);
		FName SectionName = FName();
		switch (Selection)
		{
		case 0:
			SectionName = FName("Attack1");
			break;
		case 1:
			SectionName = FName("Attack2");
			break;
		case 2:
			SectionName = FName("Attack4");
			break;
		case 3:
			SectionName = FName("Attack4");
			break;
		default:
			break;
		}

		AnimInstance->Montage_JumpToSection(SectionName, AttackMontage);
		
	}
}

void AGameProjectCharacter::PlayEquipUnequipMontage(const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && EquipUnequipMontage)
	{
		AnimInstance->Montage_Play(EquipUnequipMontage);
		AnimInstance->Montage_JumpToSection(SectionName, EquipUnequipMontage);

	}
}

void AGameProjectCharacter::AttackEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}

bool AGameProjectCharacter::CanAttack()
{
	return ActionState == EActionState::EAS_Unoccupied &&
		CharacterState != ECharacterState::ECS_Unequipped;
	
}


bool AGameProjectCharacter::CanDisarm()
{
	return ActionState == EActionState::EAS_Unoccupied 
						  && CharacterState != ECharacterState::ECS_Unequipped;
}

bool AGameProjectCharacter::CanArm()
{
	return ActionState == EActionState::EAS_Unoccupied
		   && CharacterState == ECharacterState::ECS_Unequipped
		   && EquippedWeapon;
}

void AGameProjectCharacter::Disarm()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("SpineSocket"));
	}
}

void AGameProjectCharacter::Arm()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("hand_r_socket"));
	}
}

void AGameProjectCharacter::FinishEquipping()
{
	ActionState = EActionState::EAS_Unoccupied;
}

// Called every frame
void AGameProjectCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AGameProjectCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		EnhancedInputComponent->BindAction(EchoMovementAction, ETriggerEvent::Triggered, this, &AGameProjectCharacter::Move);
		EnhancedInputComponent->BindAction(EchoLookAction, ETriggerEvent::Triggered, this, &AGameProjectCharacter::Look);
		EnhancedInputComponent->BindAction(EchoJumpAction, ETriggerEvent::Triggered, this, &AGameProjectCharacter::Jump);
		EnhancedInputComponent->BindAction(EchoEKeyPressedAction, ETriggerEvent::Triggered, this, &AGameProjectCharacter::EKeyPressed);
		EnhancedInputComponent->BindAction(EchoAttackAction, ETriggerEvent::Triggered, this, &AGameProjectCharacter::Attack);
		EnhancedInputComponent->BindAction(EchoDodgeAction, ETriggerEvent::Triggered, this, &AGameProjectCharacter::Dodge);
	}

}


