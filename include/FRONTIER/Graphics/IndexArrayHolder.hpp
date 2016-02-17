////////////////////////////////////////////////////////////////////////// <!--
/// Copyright (C) 2014-2015 Frontier (fr0nt13r789@gmail.com)           ///
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
#ifndef FRONTIER_INDEXARRAYHOLDER_HPP_INCLUDED
#define FRONTIER_INDEXARRAYHOLDER_HPP_INCLUDED

#include <FRONTIER/System/macros/TYPES.hpp>
#include <FRONTIER/System/macros/SIZE.hpp>
#include <FRONTIER/System/macros/API.h>

#define FRONTIER_INDEXARRAYHOLDER

namespace fg
{
	/////////////////////////////////////////////////////////////
	class FRONTIER_API IndexArrayHolder
	{
		void *m_ptr;
		bool m_use16bits;
		fm::Size m_size;
	public:

		explicit IndexArrayHolder(fm::Size N,bool use16bits = true);

		IndexArrayHolder(const fm::Uint32 *arrayToCopy,fm::Size indexCount);

		IndexArrayHolder(const IndexArrayHolder &other);

		~IndexArrayHolder();

		void set(fm::Size index,fm::Uint32 val);

		fm::Uint32 get(fm::Size index) const;

		fm::Size getSize() const;
		bool doUse16bits() const;
		const void *getPtr() const;
		void *getPtr();
	};
}

#endif // FRONTIER_INDEXARRAYHOLDER_HPP_INCLUDED
