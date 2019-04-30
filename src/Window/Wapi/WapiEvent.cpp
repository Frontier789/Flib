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
#include <FRONTIER/Window/Wapi/WapiWindow.hpp>
#include <FRONTIER/System/Delegate.hpp>
#include <FRONTIER/Graphics/Image.hpp>
#include <FRONTIER/Window/Event.hpp>
#include <windows.h>

namespace fw
{
	/////////////////////////////////////////////////////////////
	unsigned int VKfromButton(Mouse::Button button)
	{
		if (button == Mouse::Left)   return VK_LBUTTON;
		if (button == Mouse::Right)  return VK_RBUTTON;
		if (button == Mouse::Middle) return VK_MBUTTON;

		return 0;
	}

	/////////////////////////////////////////////////////////////
	unsigned int VKfromKey(Keyboard::Key key)
	{
		if (key == Keyboard::Left)         return VK_LEFT;
		if (key == Keyboard::Right)        return VK_RIGHT;
		if (key == Keyboard::Down)         return VK_DOWN;
		if (key == Keyboard::Up)           return VK_UP;
		if (key == Keyboard::Escape)       return VK_ESCAPE;
		if (key == Keyboard::Enter)        return VK_RETURN;
		if (key == Keyboard::PrintScreen)  return VK_SNAPSHOT;
		if (key == Keyboard::ScrollLock)   return VK_SCROLL;
		if (key == Keyboard::PauseBreak)   return VK_PAUSE;
		if (key == Keyboard::Backspace)    return VK_BACK;
		if (key == Keyboard::Insert)       return VK_INSERT;
		if (key == Keyboard::Delete)       return VK_DELETE;
		if (key == Keyboard::Home)         return VK_HOME;
		if (key == Keyboard::End)          return VK_END;
		if (key == Keyboard::PageUp)       return VK_PRIOR;
		if (key == Keyboard::PageDown)     return VK_NEXT;
		if (key == Keyboard::Divide)       return VK_DIVIDE;
		if (key == Keyboard::Multiply)     return VK_MULTIPLY;
		if (key == Keyboard::Minus)        return 0x6D;
		if (key == Keyboard::Plus)         return 0x6B;
		if (key == Keyboard::Comma)        return 0x6E;
		if (key == Keyboard::Tab)          return VK_TAB;
		if (key == Keyboard::CapsLock)     return VK_CAPITAL;
		if (key == Keyboard::LShift)       return VK_LSHIFT;
		if (key == Keyboard::RShift)       return VK_RSHIFT;
		if (key == Keyboard::LCtrl)        return VK_LCONTROL;
		if (key == Keyboard::RCtrl)        return VK_RCONTROL;
		if (key == Keyboard::LSuper)       return VK_LWIN;
		if (key == Keyboard::RSuper)       return VK_RWIN;
		if (key == Keyboard::Print)        return VK_PRINT;
		if (key == Keyboard::LAlt)         return VK_LMENU;
		if (key == Keyboard::RAlt)         return VK_RMENU;
		if (key == Keyboard::Space)        return VK_SPACE;

		if (key>=Keyboard::Num0 && key<=Keyboard::Num9)
			return 0x30+(key-Keyboard::Num0);

		if (key>=Keyboard::Numpad0 && key<=Keyboard::Numpad9)
			return VK_NUMPAD0+(key-Keyboard::Numpad0);

		if (key>=Keyboard::F1 && key<=Keyboard::F12)
			return VK_F1+(key-Keyboard::F1);

		if (key>=Keyboard::A && key<=Keyboard::Z)
			return 0x41+(key-Keyboard::A);

		return 0;
	}

	/////////////////////////////////////////////////////////////
	bool Keyboard::isKeyHeld(Keyboard::Key code)
	{
		int VKcode = VKfromKey(code);
		if (!VKcode)
			return false;
		return (GetAsyncKeyState(VKcode) & 0x8000);
	}

	/////////////////////////////////////////////////////////////
	bool Mouse::isButtonHeld(Mouse::Button button)
	{
		int VKcode = VKfromButton(button);
		if (!VKcode)
			return false;
		return (GetAsyncKeyState(VKcode) & 0x8000);
	}

    /////////////////////////////////////////////////////////////
    void Mouse::setCursor(Mouse::Cursor cursor)
    {
        LPCTSTR data[(fm::Size)Mouse::CursorCount] = 
        {
        	IDC_ARROW,
        	IDC_IBEAM,
        	IDC_HAND,
        	IDC_NO,
        	IDC_WAIT,
        	IDC_SIZENS,
        	IDC_SIZEWE,
        	IDC_SIZENS,
        	IDC_SIZEWE,
        	IDC_SIZENESW,
        	IDC_SIZENWSE,
        	IDC_SIZENESW,
        	IDC_SIZENWSE
        };

        fm::Size index = (fm::Size)cursor;

        if (index >= (fm::Size)Mouse::CursorCount) index = 0;

        SetCursor(LoadCursor(NULL,data[index]));
    }

	/////////////////////////////////////////////////////////////
	fm::vec2i Mouse::getPosition()
	{
		POINT p;
		if (!GetCursorPos(&p))
			return fm::vec2u();

		return fm::vec2u(p.x,p.y);
	}

	/////////////////////////////////////////////////////////////
	fm::vec2i Mouse::getPosition(const fw::priv::Window &window)
	{
		POINT p;
		if (!GetCursorPos(&p))
			return fm::vec2i();

		if (!ScreenToClient(window.getHandle(), &p))
			return fm::vec2i();

		return fm::vec2i(p.x,p.y);
	}

	/////////////////////////////////////////////////////////////
	void Mouse::setPosition(fm::vec3 pos)
	{
		SetCursorPos(pos.x,pos.y);
	}

	/////////////////////////////////////////////////////////////
	void Mouse::setPosition(fm::vec3 pos,const fw::priv::Window &window)
	{
		fm::vec2i winPos;
		window.getPosition(winPos.w,winPos.h);
		Mouse::setPosition(pos+winPos);
	}
	
	namespace Wapi
	{
		void imageToMasks(const fg::Image &image,fm::Uint8 transparencyLimit,
						  HBITMAP &hAndMaskBitmap, HBITMAP &hXorMaskBitmap)
		{
			HDC hDC        = ::GetDC(NULL);
			HDC hAndMaskDC = ::CreateCompatibleDC(hDC); 
			HDC hXorMaskDC = ::CreateCompatibleDC(hDC);
			
			hAndMaskBitmap	= ::CreateCompatibleBitmap(hDC,image.getSize().w,image.getSize().h);
			hXorMaskBitmap	= ::CreateCompatibleBitmap(hDC,image.getSize().w,image.getSize().h);

			HBITMAP hOldAndMaskBitmap = (HBITMAP)::SelectObject(hAndMaskDC,hAndMaskBitmap);
			HBITMAP hOldXorMaskBitmap = (HBITMAP)::SelectObject(hXorMaskDC,hXorMaskBitmap);
			
			image.forEach([&](fm::vec2s p,fg::Color c){
				if(c.a < transparencyLimit)
				{
					::SetPixel(hAndMaskDC,p.x,p.y,RGB(255,255,255));
					::SetPixel(hXorMaskDC,p.x,p.y,RGB(0,0,0));
				}
				else
				{
					::SetPixel(hAndMaskDC,p.x,p.y,RGB(0,0,0));
					::SetPixel(hXorMaskDC,p.x,p.y,RGB(c.r,c.g,c.b));
				}
			});
			
			::SelectObject(hAndMaskDC,hOldAndMaskBitmap);
			::SelectObject(hXorMaskDC,hOldXorMaskBitmap);

			::DeleteDC(hXorMaskDC);
			::DeleteDC(hAndMaskDC);

			::ReleaseDC(NULL,hDC);
		}

		HCURSOR imageToHCursor(const fg::Image &image,
							   fm::vec2s clickSpot = fm::vec2s(),
							   fm::Uint8 transparencyLimit = 128)
		{
			if (!image.getSize().area()) return NULL;
			
			HCURSOR hRetCursor = NULL;
			
			HBITMAP hAndMask = NULL;
			HBITMAP hXorMask = NULL;
			imageToMasks(image,transparencyLimit,hAndMask,hXorMask);
			
			if(!hAndMask || !hXorMask) return NULL;

			ICONINFO iconinfo   = {0,0,0,0,0};
			iconinfo.fIcon		= FALSE;
			iconinfo.xHotspot	= clickSpot.x;
			iconinfo.yHotspot	= clickSpot.y;
			iconinfo.hbmMask	= hAndMask;
			iconinfo.hbmColor	= hXorMask;

			hRetCursor = ::CreateIconIndirect(&iconinfo);
			
			return hRetCursor;
		}
		
		HCURSOR &getMouseCursorHandle(void *impl)
		{
			return *((HCURSOR*)impl);
		}
		
		void deleteMouseCursorHandle(void *&impl)
		{
			if (impl)
			{
				::DestroyIcon(getMouseCursorHandle(impl));
				
				delete (HCURSOR*)impl;
				
				impl = nullptr;
			}
		}
		
		void newMouseCursorHandle(void *&impl)
		{
			if (impl)
				deleteMouseCursorHandle(impl);
			
			impl = new HCURSOR(0);
		}
	}
	
	
	/////////////////////////////////////////////////////////////
	MouseCursor::~MouseCursor()
	{
		Wapi::deleteMouseCursorHandle(m_impl);
	}
	
	/////////////////////////////////////////////////////////////
	void MouseCursor::loadFromImage(const fg::Image &img,
					   fm::vec2s clickSpot,
					   fm::Uint8 transparencyLimit)
	{
		Wapi::newMouseCursorHandle(m_impl);
		
		Wapi::getMouseCursorHandle(m_impl) = Wapi::imageToHCursor(img,clickSpot,transparencyLimit);
	}
	
	/////////////////////////////////////////////////////////////
	void MouseCursor::setAsCurrent() const
	{
		if (m_impl)
			SetCursor(Wapi::getMouseCursorHandle(m_impl));
	}
}
