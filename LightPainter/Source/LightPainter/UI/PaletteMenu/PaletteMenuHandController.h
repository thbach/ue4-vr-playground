// Copyright TB 2020

#pragma once

#include "CoreMinimal.h"
#include "../../HandControllerBase.h"
#include "Components/WidgetComponent.h"
#include "PaletteMenuHandController.generated.h"

/**
 *
 */
UCLASS()
class LIGHTPAINTER_API APaletteMenuHandController : public AHandControllerBase
{
	GENERATED_BODY()

public:
	APaletteMenuHandController();

private:
	// Components
	UPROPERTY(VisibleAnywhere)
	UWidgetComponent* PaletteMenu;
};
