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
#ifndef FRONTIER_DELEGATE_HPP_INCLUDED
#define FRONTIER_DELEGATE_HPP_INCLUDED
#include <FRONTIER/System/type_traits/Enable_if.hpp>
#include <FRONTIER/System/type_traits/Identity.hpp>
#include <FRONTIER/System/NonCopyable.hpp>
#include <FRONTIER/System/NullPtr.hpp>

namespace fm
{
    namespace priv
    {
        class NoClass {};
    }
}

#define FRONTIER_DELEGATE_TO_INCLUDE <FRONTIER/System/Delegate/Raw.hpp>
#include <FRONTIER/System/Delegate/Make.hpp>
#undef FRONTIER_DELEGATE_TO_INCLUDE



#endif //FRONTIER_DELEGATE_HPP_INCLUDED

#ifndef FRONTIER_DONT_INCLUDE_INL

    #ifndef FRONTIER_DELEGATE_INL_INCLUDED
    #define FRONTIER_DELEGATE_INL_INCLUDED

        #define FRONTIER_DELEGATE_TO_INCLUDE <FRONTIER/System/Delegate/Raw.inl>
        #include <FRONTIER/System/Delegate/Make.hpp>
        #undef FRONTIER_DELEGATE_TO_INCLUDE

    #endif // FRONTIER_DELEGATE_INL_INCLUDED

#endif // FRONTIER_DONT_INCLUDE_INL
