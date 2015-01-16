////////////////////////////////////////////////////////////////////////// <!--
/// Copyright (C) 2014 Frontier (fr0nt13r789@gmail.com)                ///
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
#ifndef FRONTIER_STORAGE_ORDER_HPP_INCLUDED
#define FRONTIER_STORAGE_ORDER_HPP_INCLUDED
#define FRONTIER_STORAGE_ORDER

namespace fm
{
	namespace MATRIX
	{
		/////////////////////////////////////////////////////////////
		/// @brief Enum that indicates the order the data is stored in matrices
		///
		/////////////////////////////////////////////////////////////
		enum StorageOrder{
			RowMajor,	///< Indicates that the data is stored in <a href="http://en.wikipedia.org/wiki/Row-major_order#Row-major_order">row-major order</a>
			ColumnMajor ///< Indicates that the data is stored in <a href="http://en.wikipedia.org/wiki/Row-major_order#Column-major_order">column-major order</a>
		};
	}
}

#endif // FRONTIER_STORAGE_ORDER_HPP_INCLUDED
