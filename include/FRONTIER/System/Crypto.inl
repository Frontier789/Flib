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
#ifndef FRONTIER_CRYPTO_INL_INCLUDED
#define FRONTIER_CRYPTO_INL_INCLUDED

#include <vector>

namespace fm
{
	/////////////////////////////////////////////////////////////
	template<class InputIt>
	std::string sha256(InputIt beg,InputIt end)
	{
		std::vector<typename std::remove_reference<decltype((*beg))>::type> v(beg,end);
		return sha256(&v[0],v.size() * sizeof(v[0]));
	}
}

#endif
