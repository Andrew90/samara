#include "stdafx.h"
#include "AutomatAdditional.h"
#include "AppBase.h"
#include "templates.hpp"

namespace AutomatAdditional
{
	void ComputeSpeed(unsigned t)
	{
		if(0 != t)
		{
			unsigned referenceOffset = Singleton<AdditionalSettingsTable>::Instance().items.get<ReferenceOffset1>().value;
			double speed = referenceOffset / t;
			wchar_t buf[128];
			wsprintf(L"Скорость движения трубы %s мм/сек"
				, Wchar_from<double, 4>(speed)()
				);
		    app.MainWindowBottomLabel(App::speed_bottom_label, buf);
		}
	}
}