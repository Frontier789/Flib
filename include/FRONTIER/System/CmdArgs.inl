////////////////////////////////////////////////////////////////////////// <!--
/// Copyright (C) 2014-2018 Frontier (fr0nt13r789@gmail.com)           ///
///                                                                    ///
/// Flib is licensed under the terms of GNU GPL.                       ///
/// Therefore you may freely use it in your project,                   ///
/// modify it, redistribute it without any warranty on the             ///
/// condition that this disclaimer is not modified/removed.            ///
/// You may not misclaim the origin of this software.                  ///
///                                                                    ///
/// If you use this software in your program/project a                 ///
/// note about it and an email for the author (fr0nt13r789@gmail.com)  ///
/// is not required but highly appreciated.                            ///
///                                                                    ///
/// You should have received a copy of GNU GPL with this software      ///
///                                                                    ///
////////////////////////////////////////////////////////////////////////// -->
#ifndef FRONTIER_CMDARGS_INL_INCLUDED
#define FRONTIER_CMDARGS_INL_INCLUDED

namespace fm
{
	/////////////////////////////////////////////////////////////
	template<class T,class>
	inline void CmdArgs::addSwitch(fm::String argSwitch,fm::Delegate<bool,T> callback)
	{
		m_callbacks[argSwitch] = [&,callback](fm::Result &res,fm::Size &id) -> bool {
			
			++id;
			if (id == getArgCount())
			{
				res = fm::Result("CMDError",fm::Result::OPFailed,"MissingValue","processSwitch",__FILE__,__LINE__,"value was not specified for switch " + argSwitch.str());
				return false;
			}
		
			T val;
			res = StringDecoder<T>::decode(getArg(id),val);

			if (!res) return false;

			bool cb = callback(val);

			if (!cb)
				res = fm::Result("CMDError",fm::Result::OPFailed,"UnaccpetedValue","processSwitch",__FILE__,__LINE__,"Unacceptable value was given to " + argSwitch.str());

			return cb;
		};
	}
}

#endif
