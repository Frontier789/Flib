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
#ifndef FRONTIER_CAMERA_HPP_INCLUDED
#define FRONTIER_CAMERA_HPP_INCLUDED

#include <FRONTIER/System/macros/dont_include_inl_begin>

#include <FRONTIER/System/Vector3.hpp>
#include <FRONTIER/System/Matrix.hpp>
#include <FRONTIER/System/Angle.hpp>
#include <FRONTIER/System/Rect.hpp>

#include <FRONTIER/System/macros/dont_include_inl_end>

#define FRONTIER_CAMERA

namespace fm
{
	/////////////////////////////////////////////////////////////
	/// @brief Class used to handle a 3D or Orthographic Camera
	///
	/////////////////////////////////////////////////////////////
	class FRONTIER_API Camera
	{
		vec2  m_screenSize; ///< The size of the "canvas" of the camera in pixels
		vec3  m_pos;        ///< The position of the camera in world-space
		Angle m_fov;        ///< The field of view (if in 3D mode)
		float m_viewWidth;  ///< The width of the "canvas" in world-space units
		float m_znear;      ///< The distance of the near clipping plane
		float m_zfar;       ///< The distance of the far  clipping plane
		bool  m_3d;         ///< Indicates whether the camera uses perspective projection or orthographic

		mutable vec3  m_viewDir; ///< The 3D direction the camera faces
		mutable Angle m_pitch;   ///< The rotation on the X (right) axis
		mutable Angle m_yaw;     ///< The rotation on the Y (up) axis

		void anglesFromDir() const; ///< Internal function that calculates the pitch and yaw from the viewDir
		void dirFromAngles() const; ///< Internal function that calculates the viewDir from the pitch and yaw

		mutable mat4 m_projMat;       ///< The current projection matrix
		mutable mat4 m_viewMat;       ///< The current view matrix
		mutable bool m_updateViewMat; ///< Indicates whether the view matrix shall be updated
		mutable bool m_updateProjMat; ///< Indicates whether the projection matrix shall be updated

	public:
		typedef Camera &reference;
		typedef const Camera &const_reference;

		/////////////////////////////////////////////////////////////
		/// @brief Default constructor
		///
		/// @param screenSize The size of the screen the camera projects to
		///
		/////////////////////////////////////////////////////////////
		Camera(const vec2 &screenSize = vec2());

		/////////////////////////////////////////////////////////////
		/// @brief Construct a Camera with 3D projection set up
		///
		/// @param pos The position of the camera in world-space
		/// @param lookDir The 3D direction the camera faces
		/// @param screenSize The size of the screen the camera projects to
		/// @param fov The field of view (if in 3D mode)
		/// @param znear The distance of the near clipping plane
		/// @param zfar The distance of the far clipping plane
		///
		/////////////////////////////////////////////////////////////
		Camera(const vec3 &pos,const vec3 &lookDir,const vec2 &screenSize,const Angle &fov,float znear = .1,float zfar = 100);

		/////////////////////////////////////////////////////////////
		/// @brief Construct a Camera with 2D projection set up
		///
		/// @param pos The position of the camera in world-space
		/// @param lookDir The 3D direction the camera faces
		/// @param screenSize The size of the screen the camera projects to
		/// @param viewWidth The width of the "canvas" in world-space units
		/// @param znear The distance of the near clipping plane
		/// @param zfar The distance of the far clipping plane
		///
		/////////////////////////////////////////////////////////////
		Camera(const vec3 &pos,const vec3 &lookDir,const vec2 &screenSize,float viewWidth,float znear = 0,float zfar = -100);

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
		reference set3D(const vec2 &screenSize,const Angle &fov,float znear,float zfar);

		/////////////////////////////////////////////////////////////
		/// @brief Set up a 3D camera
		///
		/// @param screenSize The size of the screen the camera projects to
		/// @param fov The field of view (if in 3D mode)
		///
		/// @return Reference to itself
		///
		/////////////////////////////////////////////////////////////
		reference set3D(const vec2 &screenSize,const Angle &fov);

		/////////////////////////////////////////////////////////////
		/// @brief Set up a 3D camera
		///
		/// @param screenSize The size of the screen the camera projects to
		/// @param viewWidth The width of the "canvas" in world-space units
		/// @param znear The distance of the near clipping plane
		/// @param zfar The distance of the far clipping plane
		///
		/// @return Reference to itself
		///
		/////////////////////////////////////////////////////////////
		reference set2D(const vec2 &screenSize,float viewWidth,float znear,float zfar);

		/////////////////////////////////////////////////////////////
		/// @brief Set up a 3D camera
		///
		/// @param screenSize The size of the screen the camera projects to
		/// @param viewWidth The width of the "canvas" in world-space units
		///
		/// @return Reference to itself
		///
		/////////////////////////////////////////////////////////////
		reference set2D(const vec2 &screenSize,float viewWidth);

		/////////////////////////////////////////////////////////////
		/// @brief Set the projection matrix directly
		///
		/// Does not change anything beside the projection matrix
		///
		/// @param projMat The new projection matrix
		///
		/// @return Reference to itself
		///
		/////////////////////////////////////////////////////////////
		reference setProjMat(const mat4 &projMat);

		/////////////////////////////////////////////////////////////
		/// @brief Set the view matrix directly
		///
		/// Does not change anything beside the projection matrix
		///
		/// @param viewMat The new view matrix
		///
		/// @return Reference to itself
		///
		/////////////////////////////////////////////////////////////
		reference setViewMat(const mat4 &viewMat);

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
		const Angle &getPitch() const;

		/////////////////////////////////////////////////////////////
		/// @brief Get the yaw of the camera
		///
		/// @return The yaw of the camera
		///
		/////////////////////////////////////////////////////////////
		const Angle &getYaw() const;

		/////////////////////////////////////////////////////////////
		/// @brief Set the pitch of the camera
		///
		/// value gets clamed into [-90,90]
		///
		/// @return Reference to itself
		///
		/////////////////////////////////////////////////////////////
		reference setPitch(const Angle &pitch);

		/////////////////////////////////////////////////////////////
		/// @brief Set the yaw of the camera
		///
		/// value gets clamed into [-180,180]
		///
		/// @return Reference to itself
		///
		/////////////////////////////////////////////////////////////
		reference setYaw(const Angle &yaw);

		/////////////////////////////////////////////////////////////
		/// @brief Add to the pitch of the camera
		///
		/// the resulting value gets clamed into [-90,90]
		///
		/// @return Reference to itself
		///
		/////////////////////////////////////////////////////////////
		reference addPitch(const Angle &delta);

		/////////////////////////////////////////////////////////////
		/// @brief Add to the yaw of the camera
		///
		/// the resulting value gets clamed into [-180,180]
		///
		/// @return Reference to itself
		///
		/////////////////////////////////////////////////////////////
		reference addYaw(const Angle &delta);
	};
}

#endif // FRONTIER_CAMERA_HPP_INCLUDED
