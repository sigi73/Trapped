// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Character.h"
#include "TrappedCharacter.generated.h"

UCLASS(config=Game)
class ATrappedCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
public:
	ATrappedCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	void Tick(float DeltaSeconds) override;

	
	void Interact();
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Interaction")
	float InteractRange;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Interaction")
	bool bIsInteracting;

protected:

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	void PrepJump();
	void StartJump();

	
	float JumpTimer;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Movement")
	float PrepJumpTimer;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Movement")
	float PrepJumpTime;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Movement")
	float MovingPrepTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool bPrepJump;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float BaseJumpVelocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MaxJumpVelocity;


	

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End of APawn interface

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

