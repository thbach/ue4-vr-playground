// Copyright TB 2020

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/WidgetComponent.h"
#include "PaintingPicker/PaintingGrid.h"
#include "PaintingPicker.generated.h"

UCLASS()
class LIGHTPAINTER_API APaintingPicker : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APaintingPicker();

	void ToggleDeleteMode();
	void AddPainting();
	void UpdateCurrentPage(int32 Offset);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	void Refresh() { RefreshSlots(); RefreshDots(); }
	void RefreshSlots();
	void RefreshDots();
	int32 GetNumberOfPages() const;

	// Conponents
	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere)
	UWidgetComponent* PaintingGrid;

	UPROPERTY(VisibleAnywhere)
	UWidgetComponent* ActionBar;

	// State
	UPROPERTY()
	UPaintingGrid* PaintingGridWidget;

	int32 CurrentPage = 0;

};
