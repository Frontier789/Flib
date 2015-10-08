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

#include <FRONTIER/System/Vector3.hpp>
#include <FRONTIER/System/Matrix.hpp>
#include <FRONTIER/System/Angle.hpp>
#include <FRONTIER/System/Rect.hpp>

namespace fm
{
	/////////////////////////////////////////////////////////////
	/// @brief Class used to handle a 3D camera
	///
	/////////////////////////////////////////////////////////////
	class Camera
	{
		vec2 m_screenSize;
		vec3 m_pos;
		Angle m_fov;
		float m_viewWidth;
		float m_znear;
		float m_zfar;
		bool m_3d;

		mutable vec3 m_viewDir;
		mutable Angle m_pitch;
		mutable Angle m_yaw;

		void anglesFromDir() const;
		void dirFromAngles() const;

		mutable mat4 m_projMat;
		mutable mat4 m_viewMat;
		mutable bool m_updateViewMat;
		mutable bool m_updateProjMat;

	public:
		typedef Camera &reference;
		typedef const Camera &const_reference;

		Camera(vec2 screenSize = vec2());

		Camera(vec3 pos,vec3 lookDir,vec2 screenSize,Angle fov,float znear = .1,float zfar = 100);
		Camera(vec3 pos,vec3 lookDir,vec2 screenSize,float viewWidth,float znear = 0,float zfar = -100);

		const mat4 &getProjMat() const;
		const mat4 &getViewMat() const;

		reference lookAt(const vec3 &target);
		reference setViewDir(const vec3 &viewDir);

		reference setPos(const vec3 &pos);
		reference movePos(const vec3 &delta);

		reference setScreenSize(const vec2 &screenSize);

		vec3 u() const;
		vec3 r() const;
		vec3 f() const;

		vec3 getViewDir() const;
		vec3 getPos() const;

		reference set3D(vec2 screenSize,Angle fov,float znear,float zfar);
		reference set3D(vec2 screenSize,Angle fov);
		reference set2D(vec2 screenSize,float viewWidth,float znear,float zfar);
		reference set2D(vec2 screenSize,float viewWidth);

		reference setProjMat(const mat4 &projMat);

		bool is3D() const;

		Angle getPitch() const;
		Angle getYaw() const;

		reference setPitch(const Angle &pitch);
		reference setYaw(const Angle &yaw);
		reference addPitch(const Angle &delta);
		reference addYaw(const Angle &delta);
	};
}

#endif // FRONTIER_CAMERA_HPP_INCLUDED
