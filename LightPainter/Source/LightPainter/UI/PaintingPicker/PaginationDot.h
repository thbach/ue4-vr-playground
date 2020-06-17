// Copyright TB 2020

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "PaginationDot.generated.h"

/**
 *
 */
UCLASS()
class LIGHTPAINTER_API UPaginationDot : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetActive(bool Active);

protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (BindWidget))
	UImage* DotImage;

	UPROPERTY(EditAnywhere)
	float DisabledOpacity = 0.3;

};
