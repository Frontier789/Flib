#include <FRONTIER/Window/Wapi/WapiWindow.hpp>
#include <FRONTIER/Window/FwLog.hpp>
#include <string>

namespace fw
{
	
	std::string WapiGetLastError()
	{
		std::string ret;
		char *errorText; // FormatMessage allocates memory automatically for us
		DWORD err=GetLastError();
		FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM|FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_IGNORE_INSERTS,
					   NULL,
					   err,
					   MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
					   (LPTSTR)&errorText,0,NULL);
		if (errorText)
		{
			// if we got any description then we convert it to string
			ret=std::string(errorText);
			::LocalFree(errorText); //we free up the allocated memory
		}
		
		while (ret.length() && (ret[ret.length()-1]=='\t' || 
								ret[ret.length()-1]==' '  || 
								ret[ret.length()-1]=='\r' || 
								ret[ret.length()-1]=='\n') ) ret.resize(ret.length()-1);
		
		return ret;
	}

	bool adjustWindowSize(int &w,int &h,DWORD style)
	{
		RECT Rct;
		Rct.top    = 0;
		Rct.left   = 0;
		Rct.right  = w;
		Rct.bottom = h;
		
		// we want to specify the client rect not the whole window rect 
		if (!AdjustWindowRect(&Rct,style,FALSE))
		{
			fw_log << "AdjustWindowRect failed (lastError=\"" << WapiGetLastError() << "\")" << std::endl;
			return false;
		}
		
		w = (Rct.right  - Rct.left);
		h = (Rct.bottom - Rct.top);
		
		return true;
	}

	namespace Wapi
	{
		////////////////////////////////////////////////////////////
		LRESULT CALLBACK Window::forwardEvent(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
		{
			return DefWindowProc(hwnd, msg, wParam, lParam);
		}
		
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
		bool Window::cleanUp()
		{
			if (m_hwnd) // if the window was valid we destroy it
			{
				if (DestroyWindow(m_hwnd))
				{
					m_hwnd = NULL;
					m_windowCount--;
			
					// If we dont have any windows left we unregister our window class
					if (!m_windowCount)
					{
						if (UnregisterClassA(FRONTIER_WINDOWS_CLASS_NAME, GetModuleHandle(NULL)))
							return true;
						else
						{
							fw_log << "UnregisterClassA failed (lastError=\"" << WapiGetLastError() << "\")" << std::endl;
							return false;
						}
					}
				}
				else
				{
					fw_log << "DestroyWindow failed (lastError=\"" << WapiGetLastError() << "\")" << std::endl;
					return false;
				}
			}
			return true;
		}
		
		////////////////////////////////////////////////////////////
		bool Window::init()
		{
			// having no window means we must register a WNDCLASS
			if (m_windowCount==0)
			{
				WNDCLASS winClass = {0};
		
				// Fill in the fields of the WNDCLASS
				winClass.style         = 0;
				winClass.lpfnWndProc   = Window::forwardEvent; // When a event occures this function is called
				winClass.cbClsExtra    = 0;
				winClass.cbWndExtra    = 0;
				winClass.hInstance     = GetModuleHandle(NULL);
				winClass.hIcon         = NULL;
				winClass.hCursor       = LoadCursor(NULL, IDC_ARROW);
				winClass.hbrBackground = 0;
				winClass.lpszMenuName  = NULL;
				winClass.lpszClassName = FRONTIER_WINDOWS_CLASS_NAME; // The name of the class
				
				// Tell windows we have a class
				if (RegisterClassA(&winClass))
					return true;
				else
				{
					fw_log << "RegisterClassA failed (lastError=\"" << WapiGetLastError() << "\")" << std::endl;
					return false;
				}
			}
			
			return true;
		}
		
		////////////////////////////////////////////////////////////
		bool Window::open(int x,int y,int w,int h,const char *title,unsigned int style)
		{
			// clean our resources before recreating
			cleanUp();
			init();
			
			DWORD createStyle = WS_OVERLAPPEDWINDOW;
	
			if (!adjustWindowSize(w,h,createStyle))
				return false;
			
			// initialize the window
			m_hwnd = CreateWindowA(FRONTIER_WINDOWS_CLASS_NAME,
								   title,
								   createStyle,
								   x,y,w,h,
								   NULL,NULL,NULL,NULL);
			
			if(!m_hwnd)
			{
				// yet again if we fail...
				fw_log << "CreateWindow failed (lastError=\"" << WapiGetLastError() << "\")" << std::endl;
				return false;
			}
			
			// Tell windows to show our window
			ShowWindow(m_hwnd, SW_SHOW);
				
			m_windowCount++;
			
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
