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
#ifndef FRONTIER_TRANSFORMABLE_HPP_INCLUDED
#define FRONTIER_TRANSFORMABLE_HPP_INCLUDED

#include <FRONTIER/System/macros/dont_include_inl_begin>

#include <FRONTIER/System/Vector3.hpp>
#include <FRONTIER/System/Vector2.hpp>
#include <FRONTIER/System/Matrix.hpp>
#include <FRONTIER/System/Angle.hpp>

#include <FRONTIER/System/macros/dont_include_inl_end>

#include <FRONTIER/System/StorageOrder.hpp>
#include <FRONTIER/System/macros/API.h>

#define FRONTIER_TRANSFORMABLE

namespace fm
{
	////////////////////////////////////////////////////////////
	/// @brief Describes a transformation
	///
	/// @ingroup System
	///
	////////////////////////////////////////////////////////////
	class FRONTIER_API Transformable
	{
		vec3 m_position; ///< The translation
		vec3 m_scaling;  ///< The scaling
		Angle m_angle;   ///< The rotation angle
		vec3 m_axis;     ///< The axis of the rotation
		vec3 m_origin;   ///< The origin of the transformations
		mutable mat4 m_matrix;     ///< Holds the calculatd matrix
		mutable mat4 m_invMatrix;  ///< Holds the calculatd inverse matrix
		mutable bool m_calculated;    ///< False iff the matrix needs recalculating
		mutable bool m_invCalculated; ///< False iff the inverse matrix needs recalculating
	public:
		typedef Transformable &reference;
		typedef const Transformable &const_reference;

		////////////////////////////////////////////////////////////
		/// @brief Default constructor
		///
		////////////////////////////////////////////////////////////
		Transformable();

		////////////////////////////////////////////////////////////
		/// @brief Default destructor
		///
		////////////////////////////////////////////////////////////
		virtual ~Transformable();

		////////////////////////////////////////////////////////////
		/// @brief Set the translation
		///
		/// @param x The translation on the x axis
		/// @param y The translation on the y axis
		/// @param z The translation on the z axis
		///
		/// @return Reference to itself
		///
		////////////////////////////////////////////////////////////
		reference setPosition(float x,float y,float z = 0.f);

		////////////////////////////////////////////////////////////
		/// @brief Set the translation
		///
		/// @param pos The translation on the x and y axis
		/// @param z The translation on the z axis
		///
		/// @return Reference to itself
		///
		////////////////////////////////////////////////////////////
		reference setPosition(const vec2 &pos,float z = 0.f);

		////////////////////////////////////////////////////////////
		/// @brief Set the translation
		///
		/// @param pos The translation
		///
		/// @return Reference to itself
		///
		////////////////////////////////////////////////////////////
		reference setPosition(const vec3 &pos);

		////////////////////////////////////////////////////////////
		/// @brief Set the rotation
		///
		/// @param angle The angle of the rotation around the z axis
		///
		/// @return Reference to itself
		///
		////////////////////////////////////////////////////////////
		reference setRotation(const Angle &angle);

		////////////////////////////////////////////////////////////
		/// @brief Set the rotation
		///
		/// @param axis The axis of the rotation
		/// @param angle The angle of the rotation
		///
		/// @return Reference to itself
		///
		////////////////////////////////////////////////////////////
		reference setRotation(const vec3 &axis,const Angle &angle);

		////////////////////////////////////////////////////////////
		/// @brief Set the scaling
		///
		/// @param x The scaling on the x axis
		/// @param y The scaling on the y axis
		/// @param z The scaling on the z axis
		///
		/// @return Reference to itself
		///
		////////////////////////////////////////////////////////////
		reference setScale(float x,float y,float z = 1.f);

		////////////////////////////////////////////////////////////
		/// @brief Set the scaling
		///
		/// @param scaling The scaling on the x and y axis
		/// @param z The scaling on the z axis
		///
		/// @return Reference to itself
		///
		////////////////////////////////////////////////////////////
		reference setScale(const vec2 &scaling,float z = 1.f);

		////////////////////////////////////////////////////////////
		/// @brief Set the scaling
		///
		/// @param scaling The scaling
		///
		/// @return Reference to itself
		///
		////////////////////////////////////////////////////////////
		reference setScale(const vec3 &scaling);

		////////////////////////////////////////////////////////////
		/// @brief Set the origin of the translation
		///
		/// @param x The x coordinate of the origin
		/// @param y The y coordinate of the origin
		/// @param z The z coordinate of the origin
		///
		/// @return Reference to itself
		///
		////////////////////////////////////////////////////////////
		reference setOrigin(float x,float y,float z = 0.f);

		////////////////////////////////////////////////////////////
		/// @brief Set the origin of the translation
		///
		/// @param origin The origin
		/// @param z The third coordinate of the origin
		/// 
		/// @return Reference to itself
		///
		////////////////////////////////////////////////////////////
		reference setOrigin(const vec2 &origin,float z = 0.f);

		////////////////////////////////////////////////////////////
		/// @brief Set the origin of the translation
		///
		/// @param origin The origin
		///
		/// @return Reference to itself
		///
		////////////////////////////////////////////////////////////
		reference setOrigin(const vec3 &origin);

		////////////////////////////////////////////////////////////
		/// @brief Get the translation
		///
		/// @return The translation
		///
		////////////////////////////////////////////////////////////
		vec3 getPosition() const;

		////////////////////////////////////////////////////////////
		/// @brief Get the rotation
		///
		/// @param axis Returns the axis of the rotation
		///
		/// @return The angle of the rotation
		///
		////////////////////////////////////////////////////////////
		Angle getRotation(vec3 *axis = 0) const;

		////////////////////////////////////////////////////////////
		/// @brief Get the scaling
		///
		/// @return The scaling
		///
		////////////////////////////////////////////////////////////
		vec3 getScale() const;

		////////////////////////////////////////////////////////////
		/// @brief Get the origin
		///
		/// @return The origin
		///
		////////////////////////////////////////////////////////////
		vec3 getOrigin() const;

		////////////////////////////////////////////////////////////
		/// @brief Change the translation
		///
		/// @param x The difference on the x axis
		/// @param y The difference on the y axis
		/// @param z The difference on the z axis
		///
		/// @return Reference to itself
		///
		////////////////////////////////////////////////////////////
		reference move(float x,float y,float z = 0.f);

		////////////////////////////////////////////////////////////
		/// @brief Change the translation
		///
		/// @param pos The difference
		///
		/// @return Reference to itself
		///
		////////////////////////////////////////////////////////////
		reference move(const vec3 &pos);

		////////////////////////////////////////////////////////////
		/// @brief Change the angle of the rotation
		///
		/// @param angle The change of the angle
		///
		/// @return Reference to itself
		///
		////////////////////////////////////////////////////////////
		reference rotate(const Angle &angle);

		////////////////////////////////////////////////////////////
		/// @brief Change the scaling
		///
		/// @param x The coefficient on the x axis
		/// @param y The coefficient on the y axis
		/// @param z The coefficient on the z axis
		///
		/// @return Reference to itself
		///
		////////////////////////////////////////////////////////////
		reference scale(float x,float y,float z = 1.f);

		////////////////////////////////////////////////////////////
		/// @brief Change the scaling
		///
		/// @param scaling The coefficient on the x and y axis
		/// @param z The coefficient on the z axis
		///
		/// @return Reference to itself
		///
		////////////////////////////////////////////////////////////
		reference scale(const vec2 &scaling,float z = 1.f);

		////////////////////////////////////////////////////////////
		/// @brief Change the scaling
		///
		/// @param scaling The coefficient
		///
		/// @return Reference to itself
		///
		////////////////////////////////////////////////////////////
		reference scale(const vec3 &scaling);

		////////////////////////////////////////////////////////////
		/// @brief Get the matrix corresponding to the transformation
		///
		/// The result is in row major order
		///
		/// @return The matrix
		///
		////////////////////////////////////////////////////////////
		const mat4 &getTransform() const;

		////////////////////////////////////////////////////////////
		/// @brief Get the inverse of the matrix corresponding to the transformation
		///
		/// The result is in row major order
		///
		/// @return The matrix
		///
		////////////////////////////////////////////////////////////
		const mat4 &getInverseTransform() const;

		////////////////////////////////////////////////////////////
		/// @brief Reset the transformation to identity
		///
		/// @return Reference to itself
		///
		////////////////////////////////////////////////////////////
		reference resetTransform();

	};
}
#endif //FRONTIER_TRANSFORMABLE_HPP_INCLUDED
