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
#include <FRONTIER/Window/Xlib/XlibWindow.hpp>
#include <FRONTIER/Window/Event.hpp>
#include <FRONTIER/Window/FwLog.hpp>

#define XK_MISCELLANY
#include <X11/keysymdef.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/Xlib.h>
#include <X11/Xos.h>

namespace fw
{
	namespace priv
	{
		class XConnection
		{
		public:
			Display *disp;
			::Window root;

			XConnection() : disp(XOpenDisplay(0)),
							root(XRootWindow(disp,0))
			{

			}

			~XConnection()
			{
				if (disp)
					XCloseDisplay(disp);
			}

			operator bool() const
			{
				return disp!=NULL;
			}
		};
		static const priv::XConnection xCon;
	}
	
	/////////////////////////////////////////////////////////////
	unsigned int BMfromButton(Mouse::Button button)
	{
		if (button == Mouse::Left)   return Button1Mask;
		if (button == Mouse::Right)  return Button3Mask;
		if (button == Mouse::Middle) return Button2Mask;
		
		return 0;
	}

	/////////////////////////////////////////////////////////////
	KeyCode KCfromKey(Keyboard::Key key)
	{
		if (!priv::xCon) return 0;

		KeySym k = XK_VoidSymbol;

		if (key == Keyboard::Left)         k = XK_Left;
		if (key == Keyboard::Right)        k = XK_Right;
		if (key == Keyboard::Down)         k = XK_Down;
		if (key == Keyboard::Up)           k = XK_Up;
		if (key == Keyboard::Escape)       k = XK_Escape;
		if (key == Keyboard::Enter)        k = XK_Return;
		if (key == Keyboard::PrintScreen)  k = XK_Sys_Req;
		if (key == Keyboard::ScrollLock)   k = XK_Scroll_Lock;
		if (key == Keyboard::PauseBreak)   k = XK_Pause;
		if (key == Keyboard::Backspace)    k = XK_BackSpace;
		if (key == Keyboard::Insert)       k = XK_Insert;
		if (key == Keyboard::Delete)       k = XK_Delete;
		if (key == Keyboard::Home)         k = XK_Home;
		if (key == Keyboard::End)          k = XK_End;
		if (key == Keyboard::PageUp)       k = XK_Page_Up;
		if (key == Keyboard::PageDown)     k = XK_Page_Down;
		if (key == Keyboard::Divide)       k = XK_KP_Divide;
		if (key == Keyboard::Multiply)     k = XK_KP_Multiply;
		if (key == Keyboard::Minus)        k = XK_KP_Subtract;
		if (key == Keyboard::Plus)         k = XK_KP_Add;
		if (key == Keyboard::Comma)        k = XK_KP_Separator;
		if (key == Keyboard::Tab)          k = XK_Tab;
		if (key == Keyboard::CapsLock)     k = XK_Caps_Lock;
		if (key == Keyboard::LShift)       k = XK_Shift_L;
		if (key == Keyboard::RShift)       k = XK_Shift_R;
		if (key == Keyboard::LCtrl)        k = XK_Control_L;
		if (key == Keyboard::RCtrl)        k = XK_Control_R;
		if (key == Keyboard::LSuper)       k = XK_Super_L;
		if (key == Keyboard::RSuper)       k = XK_Super_R;
		if (key == Keyboard::Print)        k = XK_Print;
		if (key == Keyboard::LAlt)         k = XK_Alt_L;
		if (key == Keyboard::RAlt)         k = XK_Alt_R;
		if (key == Keyboard::Space)        k = XK_space;

		if (k != XK_VoidSymbol)
			return XKeysymToKeycode(priv::xCon.disp,k);
			
		if (key >= Keyboard::Num0 && key <= Keyboard::Num9)
			return XKeysymToKeycode(priv::xCon.disp,XK_0+key-Keyboard::Num0);

		if (key >= Keyboard::Numpad0 && key <= Keyboard::Numpad9)
			return XKeysymToKeycode(priv::xCon.disp,XK_KP_0+key-Keyboard::Numpad0);

		if (key >= Keyboard::F1 && key <= Keyboard::F12)
			return XKeysymToKeycode(priv::xCon.disp,XK_F1+key-Keyboard::F1);

		if (key >= Keyboard::A && key <= Keyboard::Z)
			return XKeysymToKeycode(priv::xCon.disp,XK_A+key-Keyboard::A);

		return 0;
	}
	
	/////////////////////////////////////////////////////////////
	bool Keyboard::isKeyHeld(Keyboard::Key code)
	{
		if (!priv::xCon) 
			return false;
		
		// get keyborad status
		char kbd[32];
		XQueryKeymap(priv::xCon.disp,kbd);
		
		// get KeyCode
		int offset = KCfromKey(code);
		
		// return the offset-th bit
		return kbd[offset/8] & (1<<(offset%8));
	}

	/////////////////////////////////////////////////////////////
	bool Mouse::isButtonHeld(Mouse::Button button)
	{
		if (!priv::xCon) 
			return false;
		
		::Window root,child;
		int root_x,root_y;
		int win_x,win_y;
		unsigned int mask;
		
		// retrieve a bunch of information
		XQueryPointer(priv::xCon.disp,
					  priv::xCon.root,
					  &root,&child,
					  &root_x,&root_y,
					  &win_x,&win_y,
					  &mask);
		
		return mask & BMfromButton(button);
	}
	
	/////////////////////////////////////////////////////////////
	fm::vec2i Mouse::getPosition()
	{
		if (!priv::xCon) 
			return fm::vec2i();
		
		::Window root,child;
		int root_x,root_y;
		int win_x,win_y;
		unsigned int mask;
		
		// retrieve a bunch of information
		XQueryPointer(priv::xCon.disp,
					  priv::xCon.root,
					  &root,&child,
					  &root_x,&root_y,
					  &win_x,&win_y,
					  &mask);
		
		return fm::vec2i(root_x,root_y);
	}
	
	/////////////////////////////////////////////////////////////
	fm::vec2i Mouse::getPosition(const fw::priv::Window &window)
	{
		fm::vec2i winPos;
		window.getPosition(winPos.w,winPos.h);
		return Mouse::getPosition()-winPos;
	}
	
	/////////////////////////////////////////////////////////////
	void Mouse::setPosition(const fm::vec2i &pos)
	{
		if (!priv::xCon) 
			return;
		
		XWarpPointer(priv::xCon.disp,None,priv::xCon.root,0,0,0,0,pos.x,pos.y);
		
		XFlush(priv::xCon.disp);
	}
	
	/////////////////////////////////////////////////////////////
	void Mouse::setPosition(const fm::vec2i &pos,const fw::priv::Window &window)
	{
		fm::vec2i winPos;
		window.getPosition(winPos.w,winPos.h);
		Mouse::setPosition(pos+winPos);
	}
}
