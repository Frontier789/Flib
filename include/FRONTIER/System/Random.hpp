#ifndef FRONTIER_RANDOM_HPP_INCLUDED
#define FRONTIER_RANDOM_HPP_INCLUDED
#define FRONTIER_RANDOM

#include <random>

namespace fm {
	/////////////////////////////////////////////////////////////
	/// @brief Wrapper class around the standard c++ random engine
	/// 
	/// @ingroup System
	///
	/////////////////////////////////////////////////////////////
    class Random
    {
        std::mt19937_64 m_gen; ///< The random number generator

    public:
        /////////////////////////////////////////////////////////////
        /// @brief Default constructor
        ///
        /// @param seed The seed of the generator
        ///
        /////////////////////////////////////////////////////////////
        Random(int seed = 42) : m_gen(seed) {}
        
        /////////////////////////////////////////////////////////////
        /// @brief Generate real value in a range with uniform distribution
        /// 
        /// @param beg The beginning of the interval
        /// @param end The end of the interval
        ///
        /// @return The random value
        ///
        /////////////////////////////////////////////////////////////
        double real(double beg = 0,double end = 1);
        
        /////////////////////////////////////////////////////////////
        /// @brief Generate integer value in a range with uniform distribution
        /// 
        /// @param beg The beginning of the interval
        /// @param end The end of the interval
        ///
        /// @return The random value
        ///
        /////////////////////////////////////////////////////////////
        size_t integer(size_t beg = 0,size_t end = size_t(-1));
    };
}

#endif