// Copyright TB 2020


#include "PaginationDot.h"


void UPaginationDot::SetActive(bool Active)
{
    DotImage->SetOpacity(Active ? 1 : DisabledOpacity);
}
