// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "BaseInteractable.generated.h"

UCLASS()
class TRAPPED_API ABaseInteractable : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseInteractable();

	UFUNCTION(BlueprintImplementableEvent)
	void Interact();
};
