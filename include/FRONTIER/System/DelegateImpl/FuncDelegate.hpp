////////////////////////////////////////////////////////////////////////// <!--
/// Copyright (C) 2014-2016 Frontier (fr0nt13r789@gmail.com)           ///
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
#ifndef FRONTIER_FUNCDELEGATE_HPP_INCLUDED
#define FRONTIER_FUNCDELEGATE_HPP_INCLUDED
#include <FRONTIER/System/DelegateImpl/BaseDelegate.hpp>
#define FRONTIER_FUNCDELEGATE

namespace fm
{
	/////////////////////////////////////////////////////////////
	/// @brief Class used to encapsulate a function pointer 
	///
	/////////////////////////////////////////////////////////////
	template<class AltR,class FuncArgList,class R,class... Args>
	class FuncDelegate : public priv::BaseDelegateIfCompatible<AltR,FuncArgList,R,Args...>::type
	{
	public:
		template<class... FuncArgs>
		using FuncTypeProt = AltR (*)(FuncArgs...);
		using FuncType = typename FuncArgList::template Apply<FuncTypeProt>;
		
		FuncType m_pointer; ///< Holds the pointer to the function
		
		/////////////////////////////////////////////////////////////
		/// @brief Construct the delegate from a function pointer
		/// 
		/// @param func The function pointer to be assigned
		/// 
		/////////////////////////////////////////////////////////////
		FuncDelegate(FuncType func);
		
		/////////////////////////////////////////////////////////////
		/// @brief Call the assigned function
		/// 
		/// @param callArgs The arguments to pass to the function
		/// 
		/// @return The result of the function call
		/// 
		/////////////////////////////////////////////////////////////
		R call(Args... callArgs) const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Create a proper copy using new
		/// 
		/// @return The copy created with new
		/// 
		/////////////////////////////////////////////////////////////
		BaseDelegate<R,Args...> *makeCopy() const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Check if the base delegate is nothing more than a base delegate
		/// 
		/// @return False
		/// 
		/////////////////////////////////////////////////////////////
		bool isBaseDelegate() const;
	};
}

#endif // FRONTIER_FUNCDELEGATE_HPP_INCLUDED

#ifndef FRONTIER_DONT_INCLUDE_INL
	#include <FRONTIER/System/DelegateImpl/FuncDelegate.inl>
#endif
