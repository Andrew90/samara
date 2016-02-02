#pragma once

template<class T>struct USCPpageItems
{
	typedef TL::MkTlst<
		 LongParam
		 , CrossParam
		 , ThicknessParam
	 >::Result params_list;

	typedef typename T::items_list items_list;
	
	int &unit;
	int &sens;

	TL::Factory<params_list> &params;

	template<class Z>USCPpageItems(Z &z)
		: unit(z.unit)
		, sens(z.sens)
		, params(z.items)
	{}
	
	template<class Z>Z &get()
	{
		static Z z; return z;
	}
};