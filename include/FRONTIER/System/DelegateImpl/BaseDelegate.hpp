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
#ifndef FRONTIER_BASEDELEGATE_HPP_INCLUDED
#define FRONTIER_BASEDELEGATE_HPP_INCLUDED
#include <FRONTIER/System/ArgumentList.hpp>
#define FRONTIER_BASEDELEGATE
#include <type_traits>

namespace fm
{
	/////////////////////////////////////////////////////////////
	/// @brief Class used to encapsulate a function pointer 
	///
	/////////////////////////////////////////////////////////////
	template<class R = void,class... Args>
	class BaseDelegate
	{
	public:
		typedef BaseDelegate<R,Args...> &reference;
		typedef const BaseDelegate<R,Args...> &const_reference;
		
		/////////////////////////////////////////////////////////////
		/// @brief Default destructor
		/// 
		/////////////////////////////////////////////////////////////
		virtual ~BaseDelegate();
		
		/////////////////////////////////////////////////////////////
		/// @brief Call the assigned function
		/// 
		/// This function is to be implemented by delegate implementations
		/// 
		/// @param callArgs The arguments to pass to the function
		/// 
		/// @return R()
		/// 
		/////////////////////////////////////////////////////////////
		virtual R call(Args... callArgs) const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Call the assigned function
		/// 
		/// This function is to be implemented by delegate implementations
		/// 
		/// @param callArgs The arguments to pass to the function
		/// @param extraArgs Extra arguments (discarded)
		/// 
		/// @return R()
		/// 
		/////////////////////////////////////////////////////////////
		template<class... ExtraArgs>
		R call(Args... callArgs,ExtraArgs... extraArgs) const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Create a proper copy using new
		/// 
		/// This function is to be implemented by delegate implementations
		/// 
		/// @return The copy created with new
		/// 
		/////////////////////////////////////////////////////////////
		virtual BaseDelegate<R,Args...> *makeCopy() const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Check if the base delegate is nothing more than a base delegate
		/// 
		/// @return True
		/// 
		/////////////////////////////////////////////////////////////
		virtual bool isBaseDelegate() const;
	};
	
	namespace priv
	{
		template<class LeftList,class RightList>
		class ListBeginsWith
		{
		public:
			enum {
				value = (LeftList::size != 0 && std::is_same<typename LeftList::Head,typename RightList::Head>::value && ListBeginsWith<typename LeftList::Tail,typename RightList::Tail>::value)
			};
		};

		template<class LeftList>
		class ListBeginsWith<LeftList,ArgumentList<>>
		{
		public:
			enum {
				value = 1
			};
		};

		template<class AltR,class FuncArgList,class R,class... CallArgs>
		using BaseDelegateIfCompatible = std::enable_if<ListBeginsWith<ArgumentList<CallArgs...>,FuncArgList>::value && (std::is_same<AltR,R>::value || std::is_same<void,R>::value),BaseDelegate<R,CallArgs...> >;
	}
}

#endif // FRONTIER_BASEDELEGATE_HPP_INCLUDED

#ifndef FRONTIER_DONT_INCLUDE_INL
	#include <FRONTIER/System/DelegateImpl/BaseDelegate.inl>
#endif
