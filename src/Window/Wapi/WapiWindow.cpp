#include <FRONTIER/Window/Wapi/WapiWindow.hpp>
#include <FRONTIER/Window/FwLog.hpp>

namespace fw
{
	namespace Wapi
	{
		////////////////////////////////////////////////////////////
		unsigned int Window::m_windowCount = 0;

		////////////////////////////////////////////////////////////
		Window::Window() : m_hwnd(NULL)
		{
			
		}
		
		////////////////////////////////////////////////////////////
		Window::Window(int x,int y,int w,int h,const char *title,unsigned int style) : m_hwnd(NULL)
		{
			open(x,y,w,h,title,style);
		}
		
		////////////////////////////////////////////////////////////
		Window::~Window()
		{
			cleanUp();
		}
		
		////////////////////////////////////////////////////////////
		void Window::cleanUp()
		{
			
		}
		
		////////////////////////////////////////////////////////////
		bool Window::init()
		{
			return true;
		}
		
		////////////////////////////////////////////////////////////
		bool Window::open(int x,int y,int w,int h,const char *title,unsigned int style)
		{
			// clean our mess before recreating
			cleanUp();
			if (!init())
			{
				// if init is failed we inform fw_log 
				fw_log << "Failed to create \"" << title << "\" window" << std::endl;
				return false;
			}
			
			return true;
		}
		
		////////////////////////////////////////////////////////////
		void Window::close()
		{
			cleanUp();
		}
		
		////////////////////////////////////////////////////////////
		HWND Window::getHandle() const
		{
			return m_hwnd;
		}
		
		////////////////////////////////////////////////////////////
		Window::operator HWND() const
		{
			return m_hwnd;
		}
	}
}
