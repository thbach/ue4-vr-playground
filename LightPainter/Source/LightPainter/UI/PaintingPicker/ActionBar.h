// Copyright TB 2020

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "../PaintingPicker.h"
#include "ActionBar.generated.h"

/**
 *
 */
UCLASS()
class LIGHTPAINTER_API UActionBar : public UUserWidget
{
	GENERATED_BODY()

public:
	bool Initialize() override;

	void SetParentPicker(APaintingPicker* NewParentPicker) { ParentPicker = NewParentPicker; }

protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (BindWidget))
	UButton* DeleteButton;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (BindWidget))
	UButton* AddButton;

private:
	UFUNCTION()
	void DeleteButtonClicked() { if (ParentPicker) ParentPicker->ToggleDeleteMode(); }
	UFUNCTION()
	void AddButtonClicked() { if (ParentPicker) ParentPicker->AddPainting(); }

	UPROPERTY()
	APaintingPicker* ParentPicker;

};
