typedef struct IUnknown IUnknown;

#pragma once

#include "setup.h"

namespace System
{
	VOID RedrawScreen(VOID)
	{
		RedrawWindow(NULL, NULL, NULL, RDW_INVALIDATE | RDW_ERASE | RDW_ALLCHILDREN);
	}
}