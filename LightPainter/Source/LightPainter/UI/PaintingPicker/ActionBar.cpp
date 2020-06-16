// Copyright TB 2020


#include "ActionBar.h"


bool UActionBar::Initialize()
{
    if (!Super::Initialize()) return false;
    if (!DeleteButton) return false;
    if (!AddButton) return false;

    DeleteButton->OnClicked.AddDynamic(this, &UActionBar::DeleteButtonClicked);
    AddButton->OnClicked.AddDynamic(this, &UActionBar::AddButtonClicked);

    return true;
}


