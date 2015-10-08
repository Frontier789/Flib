#include <FRONTIER/System/macros/TYPES.hpp>
#include <FRONTIER/System/Quaternion.hpp>
#include <FRONTIER/System/Polar2.hpp>
#include <FRONTIER/System/Camera.hpp>

namespace fm
{
	/////////////////////////////////////////////////////////////
	void Camera::anglesFromDir() const
	{
		vec3 viewOnPlane = vec3(m_viewDir.x,0,m_viewDir.z).sgn();

		m_pitch = fm::rad(  std::acos(viewOnPlane.dot(m_viewDir))*(m_viewDir.y < 0 ? -1 : 1)  );

		m_yaw = fm::rad(  std::acos(viewOnPlane.dot(vec3(0,0,-1)))*(m_viewDir.x < 0 ? 1 : -1)  );
	}

	/////////////////////////////////////////////////////////////
	void Camera::dirFromAngles() const
	{
		m_viewDir = fm::Quat(vec3(1,0,0),m_pitch)*fm::Quat(vec3(0,1,0),m_yaw)*vec3(0,0,-1);
	}

	/////////////////////////////////////////////////////////////
	Camera::Camera(vec2 screenSize) : m_screenSize(screenSize),
									  m_znear(0.0),
									  m_zfar(0.0),
									  m_3d(false),
									  m_updateViewMat(false),
									  m_updateProjMat(false)
	{

	}

	/////////////////////////////////////////////////////////////
	Camera::Camera(vec3 pos,vec3 lookDir,vec2 screenSize,Angle fov,float znear,float zfar) : m_screenSize(screenSize),
																							 m_pos(pos),
																							 m_fov(fov),
																							 m_viewWidth(0),
																							 m_znear(znear),
																							 m_zfar(zfar),
																							 m_3d(true),
																							 m_viewDir(lookDir.sgn()),
																							 m_updateViewMat(true),
																							 m_updateProjMat(true)
	{
		anglesFromDir();
	}

	/////////////////////////////////////////////////////////////
	Camera::Camera(vec3 pos,vec3 lookDir,vec2 screenSize,float viewWidth,float znear,float zfar) : m_screenSize(screenSize),
																								   m_pos(pos),
																								   m_viewWidth(viewWidth),
																								   m_znear(znear),
																								   m_zfar(zfar),
																								   m_3d(true),
																								   m_viewDir(lookDir.sgn()),
																								   m_updateViewMat(true),
																								   m_updateProjMat(true)
	{
		anglesFromDir();
	}


	/////////////////////////////////////////////////////////////
	const mat4 &Camera::getProjMat() const
	{
		if (m_updateProjMat)
		{
			m_updateProjMat = false;

			if (m_3d)
				m_projMat = fm::MATRIX::perspective(m_fov,m_screenSize.w/m_screenSize.h,m_znear,m_zfar);
			else
			{
				float r = m_screenSize.h/m_screenSize.w;
				m_projMat = fm::MATRIX::ortho(-m_viewWidth/2,
											  -m_viewWidth/2*r,
											   m_viewWidth/2,
											   m_viewWidth/2*r,
											   m_znear,m_zfar);
			}
		}

		return m_projMat;
	}

	/////////////////////////////////////////////////////////////
	const mat4 &Camera::getViewMat() const
	{
		if (m_updateViewMat)
		{
			m_updateViewMat = false;

			// m_viewMat = fm::MATRIX::lookAt(m_pos,m_pos+m_viewDir,u());

			m_viewMat = (fm::Quat(vec3(1,0,0),m_pitch)*fm::Quat(vec3(0,1,0),m_yaw)).getMatrix()*fm::MATRIX::translation(-m_pos);
		}

		return m_viewMat;
	}

	/////////////////////////////////////////////////////////////
	Camera::reference Camera::lookAt(const vec3 &target)
	{
		m_viewDir = (target - m_pos).sgn();

		anglesFromDir();

		m_updateViewMat = true;

		return *this;
	}

	/////////////////////////////////////////////////////////////
	Camera::reference Camera::setViewDir(const vec3 &viewDir)
	{
		m_viewDir = viewDir;

		anglesFromDir();

		m_updateViewMat = true;

		return *this;
	}


	/////////////////////////////////////////////////////////////
	Camera::reference Camera::setPos(const vec3 &pos)
	{
		m_pos = pos;

		m_updateViewMat = true;

		return *this;
	}

	/////////////////////////////////////////////////////////////
	Camera::reference Camera::movePos(const vec3 &delta)
	{
		return setPos(m_pos+delta);
	}

	/////////////////////////////////////////////////////////////
	Camera::reference Camera::setScreenSize(const vec2 &screenSize)
	{
		m_screenSize = screenSize;

		m_updateProjMat = true;

		return *this;
	}

	/////////////////////////////////////////////////////////////
	Camera::reference Camera::set3D(vec2 screenSize,Angle fov,float znear,float zfar)
	{
		m_screenSize = screenSize;
		m_fov   = fov;
		m_znear = znear;
		m_zfar  = zfar;
		m_3d    = true;

		m_updateProjMat = true;

		return *this;
	}

	/////////////////////////////////////////////////////////////
	Camera::reference Camera::set3D(vec2 screenSize,Angle fov)
	{
		m_screenSize = screenSize;
		m_fov = fov;
		m_3d  = true;

		m_updateProjMat = true;

		return *this;
	}

	/////////////////////////////////////////////////////////////
	Camera::reference Camera::set2D(vec2 screenSize,float viewWidth,float znear,float zfar)
	{
		m_screenSize = screenSize;
		m_viewWidth  = viewWidth;
		m_znear = znear;
		m_zfar  = zfar;
		m_3d    = false;

		m_updateProjMat = true;

		return *this;
	}

	/////////////////////////////////////////////////////////////
	Camera::reference Camera::set2D(vec2 screenSize,float viewWidth)
	{
		m_screenSize = screenSize;
		m_viewWidth  = viewWidth;
		m_3d = false;

		m_updateProjMat = true;

		return *this;
	}

	/////////////////////////////////////////////////////////////
	Camera::reference Camera::setProjMat(const mat4 &projMat)
	{
		m_projMat = projMat;

		m_updateProjMat = false;

		return *this;
	}


	/////////////////////////////////////////////////////////////
	vec3 Camera::u() const
	{
		return vec3(0,1,0);
	}

	/////////////////////////////////////////////////////////////
	vec3 Camera::r() const
	{
		return fm::Quat(vec3(0,1,0),m_yaw-fm::deg(90))*vec3(0,0,-1);
	}

	/////////////////////////////////////////////////////////////
	vec3 Camera::f() const
	{
		return fm::Quat(vec3(0,1,0),m_yaw)*vec3(0,0,-1);
	}

	/////////////////////////////////////////////////////////////
	vec3 Camera::getViewDir() const
	{
		return m_viewDir.sgn();
	}

	/////////////////////////////////////////////////////////////
	vec3 Camera::getPos() const
	{
		return m_pos;
	}

	/////////////////////////////////////////////////////////////
	Angle Camera::getPitch() const
	{
		return m_pitch;
	}

	/////////////////////////////////////////////////////////////
	Angle Camera::getYaw() const
	{
		double degs = m_yaw.asDeg();
		double round = degs - fm::Int64(degs/360)*360.0;

		if (round < -180) round = 360+round;
		if (round >  180) round = 360-round;

		return fm::deg(round);
	}

	/////////////////////////////////////////////////////////////
	bool Camera::is3D() const
	{
		return m_3d;
	}

	/////////////////////////////////////////////////////////////
	Camera::reference Camera::setPitch(const Angle &pitch)
	{
		m_pitch = pitch;

		if (m_pitch.asDegs() >  90) m_pitch = fm::deg( 90);
		if (m_pitch.asDegs() < -90) m_pitch = fm::deg(-90);

		dirFromAngles();

		m_updateViewMat = true;

		return *this;
	}

	/////////////////////////////////////////////////////////////
	Camera::reference Camera::setYaw(const Angle &yaw)
	{
		m_yaw = yaw;

		dirFromAngles();

		m_updateViewMat = true;

		return *this;
	}

	/////////////////////////////////////////////////////////////
	Camera::reference Camera::addPitch(const Angle &delta)
	{
		return setPitch(m_pitch+delta);
	}

	/////////////////////////////////////////////////////////////
	Camera::reference Camera::addYaw(const Angle &delta)
	{
		return setYaw(m_yaw+delta);
	}

}
