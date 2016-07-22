#include <FRONTIER/System/macros/TYPES.hpp>
#include <FRONTIER/System/MatrixStack.hpp>
#include <FRONTIER/System/Quaternion.hpp>
#include <FRONTIER/System/Vector3.hpp>
#include <FRONTIER/System/Polar2.hpp>
#include <FRONTIER/System/Camera.hpp>
#include <FRONTIER/System/Matrix.hpp>
#include <FRONTIER/System/Angle.hpp>
#include <FRONTIER/System/Rect.hpp>

namespace fm
{
	/////////////////////////////////////////////////////////////
	void Camera::anglesFromDir() const
	{
		vec3 viewOnPlane = vec3(m_viewDir.x,0,m_viewDir.z).sgn();

		m_pitch = fm::rad( std::acos(viewOnPlane.dot(m_viewDir))   *(m_viewDir.y < 0 ? -1 : 1) );
		m_yaw   = fm::rad( std::acos(viewOnPlane.dot(vec3(0,0,-1)))*(m_viewDir.x < 0 ? 1 : -1) );
	}

	/////////////////////////////////////////////////////////////
	void Camera::dirFromAngles() const
	{
		m_viewDir = fm::Quat(vec3(1,0,0),m_pitch)*fm::Quat(vec3(0,1,0),m_yaw)*vec3(0,0,-1);
	}

	/////////////////////////////////////////////////////////////
	Camera::Camera(const vec2 &screenSize) : m_screenSize(screenSize),
											 m_znear(0.0),
											 m_zfar(0.0),
											 m_3d(false),
											 m_updateViewMat(false)
	{

	}

	/////////////////////////////////////////////////////////////
	Camera::Camera(const vec3 &pos,
				   const vec3 &lookDir,
				   const vec2 &screenSize,
				   const Angle &fov,
				   float znear,
				   float zfar) : m_screenSize(screenSize),
								 m_pos(pos),
								 m_fov(fov),
								 m_viewWidth(0),
								 m_znear(znear),
								 m_zfar(zfar),
								 m_3d(true),
								 m_viewDir(lookDir.sgn()),
								 m_updateViewMat(true)
	{
		anglesFromDir();
	}

	/////////////////////////////////////////////////////////////
	Camera::Camera(const vec3 &pos,
				   const vec3 &lookDir,
				   const vec2 &screenSize,
				   float viewWidth,
				   float znear,
				   float zfar) : m_screenSize(screenSize),
								 m_pos(pos),
								 m_viewWidth(viewWidth),
								 m_znear(znear),
								 m_zfar(zfar),
								 m_3d(false),
								 m_viewDir(lookDir.sgn()),
								 m_updateViewMat(true)
	{
		anglesFromDir();
	}
	
	/////////////////////////////////////////////////////////////
	void Camera::updateProj()
	{
		m_projStack.clear();
		
		if (m_3d)
			m_projStack.push(fm::MATRIX::perspective(m_fov,m_screenSize.w/m_screenSize.h,m_znear,m_zfar));
		else
		{
			float r = m_screenSize.h/m_screenSize.w;
			m_projStack.push(fm::MATRIX::ortho(-m_viewWidth/2,
											   -m_viewWidth/2*r,
												m_viewWidth/2,
												m_viewWidth/2*r,
												m_znear,m_zfar));
		}
	}

	/////////////////////////////////////////////////////////////
	const mat4 &Camera::getProjMat() const
	{
		return m_projStack.top();
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
		m_viewDir = viewDir.sgn();

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

		updateProj();

		return *this;
	}

	/////////////////////////////////////////////////////////////
	Camera::reference Camera::set3D(const vec2 &screenSize,const Angle &fov,float znear,float zfar)
	{
		m_screenSize = screenSize;
		m_fov   = fov;
		m_znear = znear;
		m_zfar  = zfar;
		m_3d    = true;

		updateProj();

		return *this;
	}

	/////////////////////////////////////////////////////////////
	Camera::reference Camera::set2D(const vec2 &screenSize,float viewWidth,float znear,float zfar)
	{
		m_screenSize = screenSize;
		m_viewWidth  = viewWidth;
		m_znear = znear;
		m_zfar  = zfar;
		m_3d    = false;

		updateProj();

		return *this;
	}

	/////////////////////////////////////////////////////////////
	Camera::reference Camera::set2D(const vec2 &screenSize)
	{
		return set2D(screenSize,screenSize.w,-1,1);
	}
	
	/////////////////////////////////////////////////////////////
	MatrixStack<4,4,float> &Camera::getProjStack()
	{
		return m_projStack;
	}

	/////////////////////////////////////////////////////////////
	Camera::reference Camera::setViewMat(const mat4 &viewMat)
	{
		m_viewMat = viewMat;

		m_updateViewMat = false;

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
	const vec3 &Camera::getViewDir() const
	{
		return m_viewDir;
	}

	/////////////////////////////////////////////////////////////
	const vec3 &Camera::getPos() const
	{
		return m_pos;
	}

	/////////////////////////////////////////////////////////////
	const Angle &Camera::getPitch() const
	{
		return m_pitch;
	}

	/////////////////////////////////////////////////////////////
	const Angle &Camera::getYaw() const
	{
		return m_yaw;
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
		double degs = yaw.asDeg();
		double round = degs - fm::Int64(degs/360)*360.0;

		if (round < -180) round = 360+round;
		if (round >  180) round = 360-round;

		m_yaw = fm::deg(round);

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
