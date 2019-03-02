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
	/// @ingroup System
	///
	/////////////////////////////////////////////////////////////
	class FRONTIER_API Camera
	{
	public:
		enum Projection
		{
			Perspective,
			Orthogonal
		};
		
	private:
		vec3   m_up;    ///< The up vector
		vec3   m_pos;   ///< The position of the camera in world-space
		vec3   m_right; ///< The right vector
		float  m_zfar;  ///< The distance of the far  clipping plane
		float  m_znear; ///< The distance of the near clipping plane
		vec3   m_viewDir;        ///< The 3D direction the camera faces
		vec2   m_canvasSize;     ///< The size of the "canvas" of the camera in pixels
		bool   m_limitPitch;     ///< Denote whether pitch is to be limited to [-pi/2,pi/2]
		Anglef m_fieldOfView;    ///< The field of view (if in 3D mode)
		Projection m_projection; ///< Indicates whether the camera uses perspective projection or orthographic
		
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
		/// @param canvasSize The size of the imaginary canvas the camera projects to
		/// @param pos The position of the camera in world-space
		/// @param target The 3D point The camera targets
		/// @param fov The field of view (if in 3D mode)
		/// @param znear The distance of the near clipping plane
		/// @param zfar The distance of the far clipping plane
		///
		/////////////////////////////////////////////////////////////
		Camera(const vec2 &canvasSize,const vec3 &pos,const vec3 &target = vec3(0,0,-1),const Anglef &fov = fm::deg(90),float znear = .1,float zfar = 100);

		/////////////////////////////////////////////////////////////
		/// @brief Get the current view * projection (VP) matrix
		///
		/// @return The current VP matrix
		///
		/////////////////////////////////////////////////////////////
		mat4 getVPMat() const;

		/////////////////////////////////////////////////////////////
		/// @brief Get the current projection matrix
		///
		/// @return The current projection matrix
		///
		/////////////////////////////////////////////////////////////
		mat4 getProjMat() const;

		/////////////////////////////////////////////////////////////
		/// @brief Get the current view matrix
		///
		/// @return The current view matrix
		///
		/////////////////////////////////////////////////////////////
		mat4 getViewMat() const;

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
		reference setPosition(const vec3 &pos);

		/////////////////////////////////////////////////////////////
		/// @brief Move the camera
		///
		/// @param delta The offset
		///
		/// @return Reference to itself
		///
		/////////////////////////////////////////////////////////////
		reference movePosition(const vec3 &delta);

		/////////////////////////////////////////////////////////////
		/// @brief Change the virtual canvas size
		///
		/// @param canvasSize The new canvas size
		///
		/// @return Reference to itself
		///
		/////////////////////////////////////////////////////////////
		reference setCanvasSize(const vec2 &canvasSize);

		/////////////////////////////////////////////////////////////
		/// @brief Get the virtual canvas size
		///
		/// @return The canvas size
		///
		/////////////////////////////////////////////////////////////
		vec2 getCanvasSize() const;

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
		/// @brief Get the vector facing forward relative to the camera
		///
		/// @return The forward vector
		///
		/////////////////////////////////////////////////////////////
		vec3 v() const;

		/////////////////////////////////////////////////////////////
		/// @brief Get the forward vector projected onto the XZ plane 
		///
		/// @return The vector
		///
		/////////////////////////////////////////////////////////////
		vec3 f() const;

		/////////////////////////////////////////////////////////////
		/// @brief Get the vector facing forward (equals to u().cross(r()) ) relative to the camera
		///
		/// @return The forward vector
		///
		/////////////////////////////////////////////////////////////
		vec3 getViewDir() const;

		/////////////////////////////////////////////////////////////
		/// @brief Get the current position of the camera
		///
		/// @return The current position
		///
		/////////////////////////////////////////////////////////////
		vec3 getPosition() const;

		/////////////////////////////////////////////////////////////
		/// @brief Set up a 3D camera
		///
		/// @param canvasSize The size of the screen the camera projects to
		/// @param pos The position of the camera in world-space
		/// @param target The 3D point The camera targets
		/// @param fov The field of view (if in 3D mode)
		/// @param znear The distance of the near clipping plane
		/// @param zfar The distance of the far clipping plane
		///
		/// @return Reference to itself
		///
		/////////////////////////////////////////////////////////////
		reference set3D(const vec2 &canvasSize,const vec3 &pos,const vec3 &target = vec3(0,0,-1),const Anglef &fov = fm::deg(90),float znear = .1,float zfar = 100);

		/////////////////////////////////////////////////////////////
		/// @brief Create a default 2D setup
		///
		/// @param canvasSize The size of the screen the camera projects to
		/// @param znear The distance of the near clipping plane
		/// @param zfar The distance of the far clipping plane
		///
		/// @return Reference to itself
		///
		/////////////////////////////////////////////////////////////
		reference set2D(const vec2 &canvasSize,float znear = -1,float zfar = 100);
		
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
		/// @brief Get the projection method
		///
		/// @return The projection method
		///
		/////////////////////////////////////////////////////////////
		Projection getProjection() const;

		/////////////////////////////////////////////////////////////
		/// @brief Set the projection method
		///
		/// @param proj The projection method
		///
		/// @return Reference to itself
		///
		/////////////////////////////////////////////////////////////
		reference setProjection(Projection proj);

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
		/// @brief Get the roll of the camera
		///
		/// @return The roll of the camera
		///
		/////////////////////////////////////////////////////////////
		Anglef getRoll() const;

		/////////////////////////////////////////////////////////////
		/// @brief Set the pitch of the camera
		/// 
		/// @param pitch The new pitch
		/// 
		/// @return Reference to itself
		///
		/////////////////////////////////////////////////////////////
		reference setPitch(Anglef pitch);

		/////////////////////////////////////////////////////////////
		/// @brief Set the yaw of the camera
		/// 
		/// @param yaw The new yaw
		///
		/// @return Reference to itself
		///
		/////////////////////////////////////////////////////////////
		reference setYaw(Anglef yaw);

		/////////////////////////////////////////////////////////////
		/// @brief Set the roll of the camera
		/// 
		/// @param roll The new roll
		///
		/// @return Reference to itself
		///
		/////////////////////////////////////////////////////////////
		reference setRoll(Anglef roll);

		/////////////////////////////////////////////////////////////
		/// @brief Add to the pitch of the camera
		/// 
		/// @param delta The difference to add
		///
		/// @return Reference to itself
		///
		/////////////////////////////////////////////////////////////
		reference addPitch(Anglef delta);

		/////////////////////////////////////////////////////////////
		/// @brief Add to the yaw of the camera
		/// 
		/// @param delta The difference to add
		///
		/// @return Reference to itself
		///
		/////////////////////////////////////////////////////////////
		reference addYaw(Anglef delta);

		/////////////////////////////////////////////////////////////
		/// @brief Add to the roll of the camera
		/// 
		/// @param delta The difference to add
		///
		/// @return Reference to itself
		///
		/////////////////////////////////////////////////////////////
		reference addRoll(Anglef delta);
		
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
		
		/////////////////////////////////////////////////////////////
		/// @brief Get whether the pitch is being limited
		///
		/// @return True iff the pitch is being limited
		///
		/////////////////////////////////////////////////////////////
		bool getLimitPitch() const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Set whether the pitch is being limited
		///
		/// @param limit True iff the pitch is being limited
		///
		/// @return Reference to itself
		///
		/////////////////////////////////////////////////////////////
		reference setLimitPitch(bool limit);
	};
}

#endif // FRONTIER_CAMERA_HPP_INCLUDED
