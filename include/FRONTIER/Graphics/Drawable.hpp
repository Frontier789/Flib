////////////////////////////////////////////////////////////////////////// <!--
/// Copyright (C) 2014-2016 Frontier (fr0nt13r789@gmail.com)		   ///
///																	   ///
/// Flib is licensed under the terms of GNU GPL.					   ///
/// Therefore you may freely use it in your project,				   ///
/// modify it, redistribute it without any warranty on the			   ///
/// condition that this disclaimer is not modified/removed.			   ///
/// You may not misclaim the origin of this software.				   ///
///																	   ///
/// If you use this software in your program/project a				   ///
/// note about it and an email for the author (fr0nt13r789@gmail.com)  ///
/// is not required but highly appreciated.							   ///
///																	   ///
/// You should have received a copy of GNU GPL with this software	   ///
///																	   ///
////////////////////////////////////////////////////////////////////////// -->
#ifndef FRONTIER_DRAWABLE_HPP_INCLUDED
#define FRONTIER_DRAWABLE_HPP_INCLUDED
#define FRONTIER_DRAWABLE

namespace fm
{
	class Time;
}

namespace fg
{
	class ShaderManager;

	/////////////////////////////////////////////////////////////
	/// @brief Abstract base class for classes that can be drawn using a shader
	///
	/// @ingroup Graphics
	///
	/////////////////////////////////////////////////////////////
	class Drawable
	{
	public:
		/////////////////////////////////////////////////////////////
		/// @brief Default destructor
		///
		/////////////////////////////////////////////////////////////
		virtual ~Drawable();
		
		/////////////////////////////////////////////////////////////
		/// @brief Draw using a given shader
		/// 
		/// @param shader Shader to use
		/// 
		/////////////////////////////////////////////////////////////
		virtual void onDraw(ShaderManager &shader) = 0;

		/////////////////////////////////////////////////////////////
		/// @brief Regular update function
		/// 
		/// @param dt The elapsed time since last calling update
		/// 
		/////////////////////////////////////////////////////////////
		virtual void onUpdate(const fm::Time &dt) = 0;
	};
}

#endif // FRONTIER_DRAWABLE_HPP_INCLUDED
