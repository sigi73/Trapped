// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "TrappedCharacterController.generated.h"

/**
 * 
 */
UCLASS()
class TRAPPED_API ATrappedCharacterController : public APlayerController
{
	GENERATED_BODY()
	
	
public:
	ATrappedCharacterController();

protected:
	void SetupInputComponent() override;
	void Interact();
	
};
