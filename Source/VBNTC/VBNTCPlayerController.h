// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "VBNTCPlayerController.generated.h"

class UInputMappingContext;
class UUserWidget;

/**
 *  Simple first person Player Controller
 *  Manages the input mapping context.
 *  Overrides the Player Camera Manager class.
 */
UCLASS(abstract, config="Game")
class VBNTC_API AVBNTCPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:

	/** Constructor */
	AVBNTCPlayerController();

protected:
    UPROPERTY(EditAnywhere, Category="Input|Input Mappings")
    TArray<UInputMappingContext*> DefaultMappingContexts;

    UPROPERTY(EditAnywhere, Category="Input|Input Mappings")
    TArray<UInputMappingContext*> MobileExcludedMappingContexts;

    UPROPERTY(EditAnywhere, Category="UI")
    TSubclassOf<UUserWidget> MobileControlsWidgetClass;

    UPROPERTY()
    UUserWidget* MobileControlsWidget = nullptr;

    UPROPERTY(EditAnywhere, Category="Input")
    bool bForceTouchControls = false;

    virtual void BeginPlay() override;
    virtual void SetupInputComponent() override;
    bool ShouldUseTouchControls() const;
};
