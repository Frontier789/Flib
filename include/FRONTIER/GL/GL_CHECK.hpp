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
#ifndef FRONTIER_GLCHECK_HPP_INCLUDED
#define FRONTIER_GLCHECK_HPP_INCLUDED
#include <FRONTIER/System/CommonTypes.hpp>
#include <FRONTIER/System/Delegate.hpp>
#include <FRONTIER/System/util/API.h>
#include <FRONTIER/System/Result.hpp>
#define FRONTIER_GLCHECK

#ifdef FRONTIER_DEBUG
	#define glCheck(call) ((call),fg::GL::glCheckError(__FILE__,__LINE__,#call))
#else
	#define glCheck(call) ((call),fm::Result())
#endif

namespace fg
{
	namespace GL
	{
		//////////////////////////////////
		/// @brief Debug function used to check for OpenGL errors
		///
		/// If an error happened since glGetLastError was called it is returned
		/// alongside the provided data and raised using raiseGLError
		///
		/// @param file The file where the call was made
		/// @param line The line from the call was made
		/// @param call The call that maybe resulted in error
		///
		/// @return The error if any
		///
		//////////////////////////////////
		fm::Result FRONTIER_API glCheckError(const char *file, unsigned int line,const char *call = nullptr);

		//////////////////////////////////
		/// @brief Query the last GL error
		///
		/// This call pops the last error if any
		///
		/// @return The last GL error
		///
		//////////////////////////////////
		fm::Result FRONTIER_API getLastGLError();

		//////////////////////////////////
		/// @brief Check if a GL error happened
		///
		/// This call doesn't pop the last error
		///
		/// @return True iff there is a GL error
		///
		//////////////////////////////////
		bool FRONTIER_API hasGLError();

		//////////////////////////////////
		/// @brief Raise a GL error
		///
		/// @param error The error to raise
		///
		//////////////////////////////////
		void FRONTIER_API raiseGLError(const fm::Result &error);

		//////////////////////////////////
		/// @brief Set the error listener for every thread
		///
		/// @param callback The callback to use when a n error is raised
		///
		//////////////////////////////////
		void FRONTIER_API setGLErrorListener(const fm::Delegate<void,const fm::Result &> &callback);
	}
}
#endif // FRONTIER_GLCHECK_HPP_INCLUDED
