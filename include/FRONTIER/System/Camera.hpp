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
#ifndef FRONTIER_CAMERA_HPP_INCLUDED
#define FRONTIER_CAMERA_HPP_INCLUDED

#include <FRONTIER/System/util/dont_include_inl_begin>

#include <FRONTIER/System/MatrixStack.hpp>
#include <FRONTIER/System/Vector3.hpp>
#include <FRONTIER/System/Matrix.hpp>
#include <FRONTIER/System/Angle.hpp>
#include <FRONTIER/System/Rect.hpp>

#include <FRONTIER/System/util/dont_include_inl_end>

#include <FRONTIER/System/util/API.h>

#define FRONTIER_CAMERA

namespace fm
{
	/////////////////////////////////////////////////////////////
	/// @brief Class used to handle a 3D or Orthographic Camera
	///
	/////////////////////////////////////////////////////////////
	class FRONTIER_API Camera
	{
		/////////////////////////////////////////////////////////////
		/// @brief Class used hold 3d data
		///
		/////////////////////////////////////////////////////////////
		class Data3D
		{
		public:
			vec2   screenSize; ///< The size of the "canvas" of the camera in pixels
			float  znear;      ///< The distance of the near clipping plane
			float  zfar;       ///< The distance of the far  clipping plane
			vec3   pos;        ///< The position of the camera in world-space
			
			vec3   viewDir; ///< The 3D direction the camera faces
			Anglef fov;     ///< The field of view (if in 3D mode)
		};
		
		/////////////////////////////////////////////////////////////
		/// @brief Class used hold 2d data
		///
		/////////////////////////////////////////////////////////////
		class Data2D
		{
		public:
			vec2  screenSize; ///< The size of the "canvas" of the camera in pixels
			float znear;      ///< The distance of the near clipping plane
			float zfar;       ///< The distance of the far  clipping plane
			vec3  pos;        ///< The position of the camera in world-space
		};
		
		union
		{
			Data2D m_data2d; ///< The 2D mode data
			Data3D m_data3d; ///< The 3D mode data
		};
		
		bool m_3d; ///< Indicates whether the camera uses perspective projection or orthographic
		
		MatrixStack<4,4,float> m_projStack; ///< Holds the projection stack
		MatrixStack<4,4,float> m_viewStack; ///< Holds the view stack
		void updateProj(); ///< Update the projection matrix (resets the stack)
		void updateView(); ///< Update the view matrix (resets the stack)

	public:
		typedef Camera &reference;
		typedef const Camera &const_reference;

		/////////////////////////////////////////////////////////////
		/// @brief Default constructor
		///
		/////////////////////////////////////////////////////////////
		Camera();

		/////////////////////////////////////////////////////////////
		/// @brief Construct a Camera with 3D projection set up
		///
		/// @param screenSize The size of the screen the camera projects to
		/// @param pos The position of the camera in world-space
		/// @param target The 3D point The camera targets
		/// @param fov The field of view (if in 3D mode)
		/// @param znear The distance of the near clipping plane
		/// @param zfar The distance of the far clipping plane
		///
		/////////////////////////////////////////////////////////////
		Camera(const vec2 &screenSize,const vec3 &pos,const vec3 &target,const Anglef &fov = fm::deg(90),float znear = .1,float zfar = 100);

		/////////////////////////////////////////////////////////////
		/// @brief Construct a Camera with 2D projection set up
		///
		/// @param screenSize The size of the screen the camera projects to
		///
		/////////////////////////////////////////////////////////////
		Camera(const vec2 &screenSize);

		/////////////////////////////////////////////////////////////
		/// @brief Get the current projection matrix
		///
		/// @return The current projection matrix
		///
		/////////////////////////////////////////////////////////////
		const mat4 &getProjMat() const;

		/////////////////////////////////////////////////////////////
		/// @brief Get the current view matrix
		///
		/// @return The current view matrix
		///
		/////////////////////////////////////////////////////////////
		const mat4 &getViewMat() const;

		/////////////////////////////////////////////////////////////
		/// @brief Change the view direction to face @a target (to target - pos)
		///
		/// @param target The target to look at
		///
		/// @return Reference to itself
		///
		/////////////////////////////////////////////////////////////
		reference lookAt(const vec3 &target);

		/////////////////////////////////////////////////////////////
		/// @brief Set the view direction
		///
		/// @param viewDir The new direction
		///
		/// @return Reference to itself
		///
		/////////////////////////////////////////////////////////////
		reference setViewDir(const vec3 &viewDir);

		/////////////////////////////////////////////////////////////
		/// @brief Set the position
		///
		/// @param pos The new position
		///
		/// @return Reference to itself
		///
		/////////////////////////////////////////////////////////////
		reference setPos(const vec3 &pos);

		/////////////////////////////////////////////////////////////
		/// @brief Move the camera
		///
		/// @param delta The offset
		///
		/// @return Reference to itself
		///
		/////////////////////////////////////////////////////////////
		reference movePos(const vec3 &delta);

		/////////////////////////////////////////////////////////////
		/// @brief Change the screen size
		///
		/// @param screenSize The new screen size
		///
		/// @return Reference to itself
		///
		/////////////////////////////////////////////////////////////
		reference setScreenSize(const vec2 &screenSize);

		/////////////////////////////////////////////////////////////
		/// @brief Get the vector facing upward relative to the camera
		///
		/// @return The up vector
		///
		/////////////////////////////////////////////////////////////
		vec3 u() const;

		/////////////////////////////////////////////////////////////
		/// @brief Get the vector facing right relative to the camera
		///
		/// @return The right vector
		///
		/////////////////////////////////////////////////////////////
		vec3 r() const;

		/////////////////////////////////////////////////////////////
		/// @brief Get the vector facing forward (equals to u().cross(r()) ) relative to the camera
		///
		/// @return The forward vector
		///
		/////////////////////////////////////////////////////////////
		vec3 f() const;

		/////////////////////////////////////////////////////////////
		/// @brief Get the vector facing forward (equals to u().cross(r()) ) relative to the camera
		///
		/// @return The forward vector
		///
		/////////////////////////////////////////////////////////////
		const vec3 &getViewDir() const;

		/////////////////////////////////////////////////////////////
		/// @brief Get the current position of the camera
		///
		/// @return The current position
		///
		/////////////////////////////////////////////////////////////
		const vec3 &getPos() const;

		/////////////////////////////////////////////////////////////
		/// @brief Set up a 3D camera
		///
		/// @param screenSize The size of the screen the camera projects to
		/// @param fov The field of view (if in 3D mode)
		/// @param znear The distance of the near clipping plane
		/// @param zfar The distance of the far clipping plane
		///
		/// @return Reference to itself
		///
		/////////////////////////////////////////////////////////////
		reference set3D(const vec2 &screenSize,const Anglef &fov = fm::deg(90),float znear = .1,float zfar = 100);

		/////////////////////////////////////////////////////////////
		/// @brief Set up a 3D camera
		///
		/// @param screenSize The size of the screen the camera projects to
		/// @param pos The position of the camera in world-space
		/// @param target The 3D point The camera targets
		/// @param fov The field of view (if in 3D mode)
		/// @param znear The distance of the near clipping plane
		/// @param zfar The distance of the far clipping plane
		///
		/// @return Reference to itself
		///
		/////////////////////////////////////////////////////////////
		reference set3D(const vec2 &screenSize,const vec3 &pos,const vec3 &target,const Anglef &fov = fm::deg(90),float znear = .1,float zfar = 100);

		/////////////////////////////////////////////////////////////
		/// @brief Set up a 2D camera
		///
		/// @param screenSize The size of the screen the camera projects to
		/// @param znear The distance of the near clipping plane
		/// @param zfar The distance of the far clipping plane
		///
		/// @return Reference to itself
		///
		/////////////////////////////////////////////////////////////
		reference set2D(const vec2 &screenSize,float znear = -1,float zfar = 1);
		
		/////////////////////////////////////////////////////////////
		/// @brief Get the projection stack
		/// 
		/// @return The projection matrix stack
		///
		/////////////////////////////////////////////////////////////
		MatrixStack<4,4,float> &getProjStack();
		
		/////////////////////////////////////////////////////////////
		/// @brief Get the view stack
		/// 
		/// @return The view matrix stack
		///
		/////////////////////////////////////////////////////////////
		MatrixStack<4,4,float> &getViewStack();

		/////////////////////////////////////////////////////////////
		/// @brief Check if the camera is in 3D mode
		///
		/// @return True iff the camera is in 3D mode
		///
		/////////////////////////////////////////////////////////////
		bool is3D() const;

		/////////////////////////////////////////////////////////////
		/// @brief Get the pitch of the camera
		///
		/// @return The pitch of the camera
		///
		/////////////////////////////////////////////////////////////
		Anglef getPitch() const;

		/////////////////////////////////////////////////////////////
		/// @brief Get the yaw of the camera
		///
		/// @return The yaw of the camera
		///
		/////////////////////////////////////////////////////////////
		Anglef getYaw() const;

		/////////////////////////////////////////////////////////////
		/// @brief Set the pitch of the camera
		///
		/// value gets clamed into [-90,90]
		/// 
		/// @param pitch The new pitch
		/// 
		/// @return Reference to itself
		///
		/////////////////////////////////////////////////////////////
		reference setPitch(const Anglef &pitch);

		/////////////////////////////////////////////////////////////
		/// @brief Set the yaw of the camera
		///
		/// value gets clamed into [-180,180]
		/// 
		/// @param pitch The new yaw
		///
		/// @return Reference to itself
		///
		/////////////////////////////////////////////////////////////
		reference setYaw(const Anglef &yaw);

		/////////////////////////////////////////////////////////////
		/// @brief Add to the pitch of the camera
		///
		/// the resulting value gets clamed into [-90,90]
		/// 
		/// @param delta The difference to add
		///
		/// @return Reference to itself
		///
		/////////////////////////////////////////////////////////////
		reference addPitch(const Anglef &delta);

		/////////////////////////////////////////////////////////////
		/// @brief Add to the yaw of the camera
		///
		/// the resulting value gets clamed into [-180,180]
		/// 
		/// @param delta The difference to add
		///
		/// @return Reference to itself
		///
		/////////////////////////////////////////////////////////////
		reference addYaw(const Anglef &delta);
		
		/////////////////////////////////////////////////////////////
		/// @brief Set the near clipping plane distance
		/// 
		/// @param znear The new near value
		/// 
		/// @return Reference to itself
		///
		/////////////////////////////////////////////////////////////
		reference setNear(float znear);
		
		/////////////////////////////////////////////////////////////
		/// @brief Set the far clipping plane distance
		/// 
		/// @param zfar The new near value
		///
		/// @return Reference to itself
		///
		/////////////////////////////////////////////////////////////
		reference setFar(float zfar);
		
		/////////////////////////////////////////////////////////////
		/// @brief Get the current near clipping plane
		///
		/// @return The current near value
		///
		/////////////////////////////////////////////////////////////
		float getNear() const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Get the current far clipping plane
		///
		/// @return The current far value
		///
		/////////////////////////////////////////////////////////////
		float getFar() const;
	};
}

#endif // FRONTIER_CAMERA_HPP_INCLUDED
