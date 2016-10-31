#include "stdafx.h"
#include "VersionDiff.h"
#include "AppBase.h"
#include "USPCData.h"


namespace Version
{
	static const int __magic_word__ = 0xcafe0000;
	static const int __version__ = 1;

	struct ScopeVelocity
	{
		double value[App::count_sensors];
	};
	ScopeVelocity stored_items;

	void SaveToFile(FILE *f)
	{
		unsigned t = __magic_word__ | __version__;
		fwrite(&t, sizeof(unsigned), 1, f);
		ItemData<Thickness> &d = Singleton<ItemData<Thickness>>::Instance();
		fwrite(d.scope_velocity, sizeof(d.scope_velocity), 1, f);
	}

	bool LoadFromFile(unsigned ver, FILE *f)
	{
		if(__magic_word__ == (ver & 0xffff0000))
		{
			switch(ver & 0xffff)
			{
			case 1:
				{
					ItemData<Thickness> &d = Singleton<ItemData<Thickness>>::Instance();
					fread(d.scope_velocity, sizeof(d.scope_velocity), 1, f);
				}
				return true;
			}
		}
		return false;
	}

	void RestoreMemory()
	{
		ItemData<Thickness> &d = Singleton<ItemData<Thickness>>::Instance();
		memcpy(d.scope_velocity, stored_items.value, sizeof(d.scope_velocity)); 
	}
	//void StoreMemory(){}
	void StoreScopeVelocity(int i, double data)
	{
		stored_items.value[i] = data;
	}
}