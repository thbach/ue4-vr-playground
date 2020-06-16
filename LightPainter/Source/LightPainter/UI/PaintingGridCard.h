// Copyright TB 2020

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "PaintingGridCard.generated.h"

/**
 *
 */
UCLASS()
class LIGHTPAINTER_API UPaintingGridCard : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetPaintingName(FString PaintingName);

protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (BindWidget))
	UTextBlock* SlotName;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (BindWidget))
	UButton* CardButton;

private:
	// Config


	// functions
	UFUNCTION()
	void CardButtonClicked();

	// State
	FString PaintingName;

};
