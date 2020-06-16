// Copyright TB 2020

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "PainterSaveGame.h"
#include "PainterSaveGameIndex.generated.h"

/**
 *
 */
UCLASS()
class LIGHTPAINTER_API UPainterSaveGameIndex : public USaveGame
{
	GENERATED_BODY()

public:
	bool Save();
	static UPainterSaveGameIndex* Load();

	void AddSaveGame(UPainterSaveGame* SaveGame);
	TArray<FString> GetSlotNames() const { return SlotNames; }

private:
	// State
	static const FString SLOT_NAME;

	UPROPERTY()
	TArray<FString> SlotNames;

};
