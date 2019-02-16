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
#ifndef FRONTIER_DELEGATE_HPP_INCLUDED
#define FRONTIER_DELEGATE_HPP_INCLUDED
#include <FRONTIER/System/util/RequireCpp11.hpp>
#include <FRONTIER/System/DelegateImpl/BaseDelegate.hpp>
#include <FRONTIER/System/util/is_callable.hpp>
#define FRONTIER_DELEGATE

namespace fm
{
	/////////////////////////////////////////////////////////////
	/// @brief Delegate class used to encapsulate a function call
	/// 
	/// @ingroup System
	///
	/////////////////////////////////////////////////////////////
	template<class R,class... Args>
	class Delegate
	{
		BaseDelegate<R,Args...> *m_impl; ///< The implementation of the delegate
		
	public:
		typedef Delegate<R,Args...> &reference;
		typedef const Delegate<R,Args...> &const_reference;
		
		/////////////////////////////////////////////////////////////
		/// @brief Default constructor
		/// 
		/// Does not assign function
		/// 
		/////////////////////////////////////////////////////////////
		Delegate();
		
		/////////////////////////////////////////////////////////////
		/// @brief Construct from nullptr
		/// 
		/// Does not assign function
		/// 
		/////////////////////////////////////////////////////////////
		Delegate(decltype(nullptr));
		
		/////////////////////////////////////////////////////////////
		/// @brief Construct from basic delegate
		/// 
		/// Does not assign function
		/// 
		/////////////////////////////////////////////////////////////
		Delegate(BaseDelegate<R,Args...> *newBaseDelegate);
		
		/////////////////////////////////////////////////////////////
		/// @brief Copy constructor
		/// 
		/// @param copy The object to copy
		/// 
		/////////////////////////////////////////////////////////////
		Delegate(const Delegate<R,Args...> &copy);
		
		/////////////////////////////////////////////////////////////
		/// @brief Move constructor
		/// 
		/// @param move The object to move
		/// 
		/////////////////////////////////////////////////////////////
		Delegate(Delegate<R,Args...> &&move);
		
		/////////////////////////////////////////////////////////////
		/// @brief Default destructor
		/// 
		/////////////////////////////////////////////////////////////
		~Delegate();
		
		/////////////////////////////////////////////////////////////
		/// @brief Construct delegate from a functor
		/// 
		/// Copies the object
		/// 
		/// @param functor The functor to assign
		/// 
		/////////////////////////////////////////////////////////////
		template<class FunctorT>
		Delegate(const FunctorT &functor,typename std::enable_if<fm::is_callable<FunctorT>::value,void>::type* = nullptr);
		
		/////////////////////////////////////////////////////////////
		/// @brief Construct delegate from a lambda
		/// 
		/// Doas not copy the object
		/// 
		/// @param functor The functor to assign
		/// 
		/////////////////////////////////////////////////////////////
		template<class FunctorT>
		Delegate(FunctorT *functor,typename std::enable_if<fm::is_callable<FunctorT>::value,void>::type* = nullptr);
		
		/////////////////////////////////////////////////////////////
		/// @brief Construct delegate from a function
		/// 
		/// @param funcptr The function to assign
		/// 
		/////////////////////////////////////////////////////////////
		template<class AltR,class... CallArgs>
		Delegate(AltR (*funcptr)(CallArgs...));
		
		/////////////////////////////////////////////////////////////
		/// @brief Construct delegate from a function
		/// 
		/// @param memFunc The function to assign
		/// @param object The object the function will be called on
		/// 
		/////////////////////////////////////////////////////////////
		template<class ObjectType,class AltR,class... CallArgs>
		Delegate(AltR (ObjectType::*memFunc)(CallArgs...),ObjectType object = ObjectType());
		
		/////////////////////////////////////////////////////////////
		/// @brief Construct delegate from a function
		/// 
		/// @param memFunc The function to assign
		/// @param object The object the function will be called on
		/// 
		/////////////////////////////////////////////////////////////
		template<class ObjectType,class AltR,class... CallArgs>
		Delegate(AltR (ObjectType::*memFunc)(CallArgs...) const,ObjectType object = ObjectType());
		
		/////////////////////////////////////////////////////////////
		/// @brief Construct delegate from a function
		/// 
		/// @param memFunc The function to assign
		/// @param pointer Pointer to the object the function will be called on
		/// 
		/////////////////////////////////////////////////////////////
		template<class ObjectType,class AltR,class... CallArgs>
		Delegate(AltR (ObjectType::*memFunc)(CallArgs...),ObjectType *pointer);
		
		/////////////////////////////////////////////////////////////
		/// @brief Construct delegate from a function
		/// 
		/// @param memFunc The function to assign
		/// @param pointer Pointer to the object the function will be called on
		/// 
		/////////////////////////////////////////////////////////////
		template<class ObjectType,class AltR,class... CallArgs>
		Delegate(AltR (ObjectType::*memFunc)(CallArgs...),const ObjectType *pointer) = delete;
		
		/////////////////////////////////////////////////////////////
		/// @brief Construct delegate from a function
		/// 
		/// @param memFunc The function to assign
		/// @param pointer Pointer to the object the function will be called on
		/// 
		/////////////////////////////////////////////////////////////
		template<class ObjectType,class AltR,class... CallArgs>
		Delegate(AltR (ObjectType::*memFunc)(CallArgs...) const,const ObjectType *pointer);
		
		/////////////////////////////////////////////////////////////
		/// @brief Call the assigned function
		/// 
		/// @param callArgs The arguments to pass to the function
		/// @param extraArgs Extra arguments (discarded)
		/// 
		/// @return The result to the assigned function or R() if no function was assigned
		/// 
		/////////////////////////////////////////////////////////////
		template<class... ExtraArgs>
		R call(Args... callArgs,ExtraArgs... extraArgs) const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Call the assigned function
		/// 
		/// @param callArgs The arguments to pass to the function
		/// @param extraArgs Extra arguments (discarded)
		/// 
		/// @return The result to the assigned function or R() if no function was assigned
		/// 
		/////////////////////////////////////////////////////////////
		template<class... ExtraArgs>
		R operator()(Args... callArgs,ExtraArgs... extraArgs) const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Check if the delegate is assigned
		/// 
		/// @return True iff the delegate is assigned a function
		/// 
		/////////////////////////////////////////////////////////////
		operator bool() const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Check if the delegate is not assigned
		/// 
		/// @return True iff the delegate is not assigned a function
		/// 
		/////////////////////////////////////////////////////////////
		bool operator!() const;
		
		
		/////////////////////////////////////////////////////////////
		/// @brief Copy assignment
		/// 
		/// @param copy The object to copy
		/// 
		/// @return reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		Delegate<R,Args...> &operator=(const Delegate<R,Args...> &copy);
		
		/////////////////////////////////////////////////////////////
		/// @brief Move assignment
		/// 
		/// @param move The object to move
		/// 
		/// @return reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		Delegate<R,Args...> &operator=(Delegate<R,Args...> &&move);
	};
}

#endif // FRONTIER_DELEGATE_HPP_INCLUDED

#ifndef FRONTIER_DONT_INCLUDE_INL
	#include <FRONTIER/System/Delegate.inl>
#endif
