// Fill out your copyright notice in the Description page of Project Settings.

#include "Trapped.h"
#include "BaseInteractable.h"
#include "TrappedCharacter.h"
#include "TrappedCharacterController.h"



ATrappedCharacterController::ATrappedCharacterController()
{
	//bShowMouseCursor = true;
	bEnableMouseOverEvents = true;
}

void ATrappedCharacterController::SetupInputComponent()
{
	Super::SetupInputComponent();

	//InputComponent->BindAction("Interact", IE_Pressed, this, &ATrappedCharacterController::Interact);
}
/*
void ATrappedCharacterController::Interact()
{
	FHitResult HitResult;
	GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_Visibility), true, HitResult);

	if (HitResult.bBlockingHit)
	{
		ABaseInteractable* Interactable = Cast<ABaseInteractable>(HitResult.GetActor());

		ATrappedCharacter* Character = Cast<ATrappedCharacter>(GetPawn());

		if (Interactable && Character)
		{
			FVector InteractablePos = Interactable->GetActorLocation();
			FVector CharacterPos = Character->GetActorLocation();
			float Dist = FVector::Dist(InteractablePos, CharacterPos);

			if (Dist <= Character->InteractRange)
			{
				Interactable->Interact();
				Character->bIsInteracting = true;
				//Character->Interact();
			}
		}
	}
}
*/