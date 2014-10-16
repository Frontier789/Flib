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