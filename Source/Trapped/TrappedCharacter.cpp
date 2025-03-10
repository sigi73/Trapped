// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "Trapped.h"
#include "BaseInteractable.h"
#include "TrappedCharacter.h"

//////////////////////////////////////////////////////////////////////////
// ATrappedCharacter

ATrappedCharacter::ATrappedCharacter()
{

	JumpTimer = 0.0f;
	bPrepJump = false;
	PrepJumpTime = 0.6f;
	MovingPrepTime = 0.2f;
	PrepJumpTimer = PrepJumpTime;
	BaseJumpVelocity = 300.0f;
	MaxJumpVelocity = 500.0f;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = BaseJumpVelocity;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

}

//////////////////////////////////////////////////////////////////////////
// Input

void ATrappedCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	// Set up gameplay key bindings
	check(InputComponent);
	InputComponent->BindAction("Jump", IE_Pressed, this, &ATrappedCharacter::PrepJump);
	InputComponent->BindAction("Jump", IE_Released, this, &ATrappedCharacter::StartJump);
	//InputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	InputComponent->BindAxis("MoveForward", this, &ATrappedCharacter::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ATrappedCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	InputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	InputComponent->BindAxis("TurnRate", this, &ATrappedCharacter::TurnAtRate);
	InputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	InputComponent->BindAxis("LookUpRate", this, &ATrappedCharacter::LookUpAtRate);

	// handle touch devices
	InputComponent->BindTouch(IE_Pressed, this, &ATrappedCharacter::TouchStarted);
	InputComponent->BindTouch(IE_Released, this, &ATrappedCharacter::TouchStopped);

	InputComponent->BindAction("Interact", IE_Pressed, this, &ATrappedCharacter::Interact);

}


void ATrappedCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	// jump, but only on the first touch
	if (FingerIndex == ETouchIndex::Touch1)
	{
		Jump();
	}
}

void ATrappedCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	if (FingerIndex == ETouchIndex::Touch1)
	{
		StopJumping();
	}
}

void ATrappedCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ATrappedCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ATrappedCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f) && (!bPrepJump) && (!bIsInteracting))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ATrappedCharacter::MoveRight(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f) && (!bPrepJump) && (!bIsInteracting))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void ATrappedCharacter::PrepJump()
{
	PrepJumpTimer = 0.0f;
}

void ATrappedCharacter::StartJump()
{
	bPrepJump = false;
	Jump();
	JumpTimer = 0.0f;
	
	float JumpVelocity;
	if (PrepJumpTimer <= MovingPrepTime)
	{
		JumpVelocity = BaseJumpVelocity;
	}
	else
	{
		JumpVelocity = PrepJumpTimer / PrepJumpTime * MaxJumpVelocity;
	}
	GetCharacterMovement()->JumpZVelocity = JumpVelocity;

	PrepJumpTimer = PrepJumpTime;
}

void ATrappedCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);


	if (JumpTimer > 0.5f)
	{
		StopJumping();
	}
	else
	{
		JumpTimer += DeltaSeconds;
	}

	if (PrepJumpTimer >= PrepJumpTime)
	{
		PrepJumpTimer = PrepJumpTime;
	}
	else
	{
		
		if (PrepJumpTimer > MovingPrepTime)
		{
			bPrepJump = true;
		}
		PrepJumpTimer += DeltaSeconds;
	}
}


void ATrappedCharacter::Interact()
{
	FHitResult HitOut = FHitResult(ForceInit);
	
	GetWorld()->LineTraceSingleByChannel(HitOut, GetActorLocation(), GetActorLocation() + GetActorForwardVector() * InteractRange, ECC_Visibility);
	ABaseInteractable* Interactable = Cast<ABaseInteractable>(HitOut.GetActor());
	if (Interactable)
	{
		Interactable->Interact();
		bIsInteracting = true;
	}
}
