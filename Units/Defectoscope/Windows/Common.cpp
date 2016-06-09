#include "stdafx.h"
#include "Common.h"

#include "ThicknessWindow.h"
#include "LongWindow.h"
#include "CrossWindow.h"

void CloseAllWindows()
{
	Common::DestroyWindow<ThicknessWindow>();
	Common::DestroyWindow<LongWindow     >();
	Common::DestroyWindow<CrossWindow    >();
	Common::DestroyWindow<ScanWindow     >();
}