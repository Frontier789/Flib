#include <FRONTIER/System/CommonTypes.hpp>
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
	Camera::Camera() : m_data2d{vec2(),0,0,vec3()},
					   m_3d(false)
	{

	}

	/////////////////////////////////////////////////////////////
	Camera::Camera(const vec2 &screenSize,
				   const vec3 &pos,
				   const vec3 &target,
				   const Anglef &fov,
				   float znear,
				   float zfar) : m_data3d{screenSize,znear,zfar,pos,(target - pos).sgn(),fov},
								 m_3d(true)
	{
		updateProj();
		updateView();
	}

	/////////////////////////////////////////////////////////////
	Camera::Camera(const vec2 &screenSize) : m_data2d{screenSize,-1,1,vec3()},
											 m_3d(false)
	{
		updateProj();
		updateView();
	}
	
	/////////////////////////////////////////////////////////////
	void Camera::updateProj()
	{
		m_projStack.clear();
		
		if (m_3d)
			m_projStack.push(fm::MATRIX::perspective(m_data3d.fov,m_data3d.screenSize.w/m_data3d.screenSize.h,m_data3d.znear,m_data3d.zfar));
		else
		{
			float r = m_data2d.screenSize.h/m_data2d.screenSize.w;
			m_projStack.push(fm::MATRIX::ortho(0,
											   m_data2d.screenSize.h,
											   m_data2d.screenSize.w,
											   0,
											   m_data2d.znear,m_data2d.zfar));
		}
	}
	
	/////////////////////////////////////////////////////////////
	void Camera::updateView()
	{
		m_viewStack.clear();
		
		if (m_3d)
			m_viewStack.push(fm::MATRIX::lookAt(m_data3d.pos,m_data3d.pos+m_data3d.viewDir,u()));
		else
			m_viewStack.push(fm::MATRIX::translation(-m_data2d.pos));
	}

	/////////////////////////////////////////////////////////////
	const mat4 &Camera::getProjMat() const
	{
		return m_projStack.top();
	}

	/////////////////////////////////////////////////////////////
	const mat4 &Camera::getViewMat() const
	{
		return m_viewStack.top();
	}

	/////////////////////////////////////////////////////////////
	Camera::reference Camera::lookAt(const vec3 &target)
	{
		if (m_3d)
		{
			m_data3d.viewDir = (target - m_data3d.pos).sgn();
			
			updateView();
		}

		return *this;
	}

	/////////////////////////////////////////////////////////////
	Camera::reference Camera::setViewDir(const vec3 &viewDir)
	{
		if (m_3d)
		{
			m_data3d.viewDir = viewDir.sgn();

			updateView();
		}

		return *this;
	}


	/////////////////////////////////////////////////////////////
	Camera::reference Camera::setPos(const vec3 &pos)
	{
		m_data2d.pos = pos;

		updateView();

		return *this;
	}

	/////////////////////////////////////////////////////////////
	Camera::reference Camera::movePos(const vec3 &delta)
	{
		return setPos(m_data2d.pos+delta);
	}

	/////////////////////////////////////////////////////////////
	Camera::reference Camera::setScreenSize(const vec2 &screenSize)
	{
		m_data2d.screenSize = screenSize;

		updateProj();

		return *this;
	}

	/////////////////////////////////////////////////////////////
	Camera::reference Camera::set3D(const vec2 &screenSize,const Anglef &fov,float znear,float zfar)
	{
		m_data3d.screenSize = screenSize;
		m_data3d.znear = znear;
		m_data3d.zfar  = zfar;
		m_data3d.fov   = fov;
		
		m_data3d.viewDir = vec3(0,0,-1);
		m_data3d.pos     = vec3();
		
		m_3d = true;

		updateProj();

		return *this;
	}

	/////////////////////////////////////////////////////////////
	Camera::reference Camera::set3D(const vec2 &screenSize,const vec3 &pos,const vec3 &target,const Anglef &fov,float znear,float zfar)
	{
		set3D(screenSize,fov,znear,zfar);
		setPos(pos);
		lookAt(target);

		return *this;
	}

	/////////////////////////////////////////////////////////////
	Camera::reference Camera::set2D(const vec2 &screenSize,float znear,float zfar)
	{
		m_data2d.screenSize = screenSize;
		m_data2d.znear = znear;
		m_data2d.zfar  = zfar;
		m_data2d.pos   = vec3();
		m_3d = false;

		updateProj();

		return *this;
	}
	
	/////////////////////////////////////////////////////////////
	MatrixStack<4,4,float> &Camera::getProjStack()
	{
		return m_projStack;
	}
	
	/////////////////////////////////////////////////////////////
	MatrixStack<4,4,float> &Camera::getViewStack()
	{
		return m_projStack;
	}

	/////////////////////////////////////////////////////////////
	vec3 Camera::u() const
	{
		return vec3(0,1,0);
	}

	/////////////////////////////////////////////////////////////
	vec3 Camera::r() const
	{
		return fm::Quat(vec3(0,1,0),getYaw()-fm::deg(90))*vec3(0,0,-1);
	}

	/////////////////////////////////////////////////////////////
	vec3 Camera::f() const
	{
		return fm::Quat(vec3(0,1,0),getYaw())*vec3(0,0,-1);
	}

	/////////////////////////////////////////////////////////////
	const vec3 &Camera::getViewDir() const
	{
		return m_data3d.viewDir;
	}

	/////////////////////////////////////////////////////////////
	const vec3 &Camera::getPos() const
	{
		return m_data3d.pos;
	}

	/////////////////////////////////////////////////////////////
	Anglef Camera::getPitch() const
	{
		vec3 viewOnPlane = vec3(m_data3d.viewDir.x,0,m_data3d.viewDir.z).sgn();

		return fm::rad( std::acos(viewOnPlane.dot(m_data3d.viewDir))*(m_data3d.viewDir.y < 0 ? -1 : 1) );
	}

	/////////////////////////////////////////////////////////////
	Anglef Camera::getYaw() const
	{
		vec3 viewOnPlane = vec3(m_data3d.viewDir.x,0,m_data3d.viewDir.z).sgn();

		return fm::rad( std::acos(viewOnPlane.dot(vec3(0,0,-1)))*(m_data3d.viewDir.x < 0 ? 1 : -1) );
	}

	/////////////////////////////////////////////////////////////
	bool Camera::is3D() const
	{
		return m_3d;
	}

	/////////////////////////////////////////////////////////////
	Camera::reference Camera::setPitch(const Anglef &pitch)
	{
		Anglef pitchR = pitch;

		if (pitchR.asDeg() >  89.99) pitchR = fm::deg( 89.99);
		if (pitchR.asDeg() < -89.99) pitchR = fm::deg(-89.99);

		m_data3d.viewDir = fm::Quat(vec3(1,0,0),pitchR)*fm::Quat(vec3(0,1,0),getYaw())*vec3(0,0,-1);

		updateView();

		return *this;
	}
	
	/////////////////////////////////////////////////////////////
	Camera::reference Camera::setYaw(const Anglef &yaw)
	{
		Anglef yawR = fm::deg(std::fmod(yaw.asDeg(),360));

		if (yawR.asDeg() < -180) yawR = fm::deg(360)+yawR;
		if (yawR.asDeg() >  180) yawR = yawR-fm::deg(360);

		m_data3d.viewDir = fm::Quat(vec3(1,0,0),getPitch())*fm::Quat(vec3(0,1,0),yawR)*vec3(0,0,-1);

		updateView();

		return *this;
	}

	/////////////////////////////////////////////////////////////
	Camera::reference Camera::addPitch(const Anglef &delta)
	{
		return setPitch(getPitch()+delta);
	}

	/////////////////////////////////////////////////////////////
	Camera::reference Camera::addYaw(const Anglef &delta)
	{
		return setYaw(getYaw()+delta);
	}
	
	/////////////////////////////////////////////////////////////
	Camera::reference Camera::setNear(float znear)
	{
		m_data2d.znear = znear;
		
		updateProj();
		
		return *this;
	}
	
	/////////////////////////////////////////////////////////////
	Camera::reference Camera::setFar(float zfar)
	{
		m_data2d.zfar = zfar;
		
		updateProj();
		
		return *this;
	}
	
	/////////////////////////////////////////////////////////////
	float Camera::getNear() const
	{
		return m_data2d.znear;
	}
	
	/////////////////////////////////////////////////////////////
	float Camera::getFar() const
	{
		return m_data2d.zfar;
	}
}
