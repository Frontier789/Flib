#include <FRONTIER/System/Clipboard.hpp>
#include <FRONTIER/System/util/C.hpp>
#include <windows.h>

namespace fm
{
	/////////////////////////////////////////////////////////////
	namespace clipboard
	{
		/////////////////////////////////////////////////////////////
		fm::Result open()
		{
			if (!OpenClipboard(nullptr))
				return fm::Result("IOError",fm::Result::OPFailed,"ClipboardNotAvailable","openClipboard",__FILE__,__LINE__);

			return {};
		}

		/////////////////////////////////////////////////////////////
		fm::Result close()
		{
			CloseClipboard();
			
			return {};
		}

		/////////////////////////////////////////////////////////////
		fm::Result clear()
		{
			EmptyClipboard();
			
			return {};
		}

		/////////////////////////////////////////////////////////////
		bool hasTextData()
		{
			return IsClipboardFormatAvailable(CF_UNICODETEXT) || IsClipboardFormatAvailable(CF_TEXT);
		}

		/////////////////////////////////////////////////////////////
		fm::String getTextData()
		{
			if (IsClipboardFormatAvailable(CF_UNICODETEXT))
			{
				fm::String ret;
				HANDLE h = GetClipboardData(CF_UNICODETEXT);

				if (h)
				{
					char *ptr = (char*)GlobalLock(h);
					
					if (ptr)
					{
						int buflen = 0;
						
						for (char *it = ptr;*it || *(it+1);it+=2,buflen+=2);
						
						// change UTF16-LE to UTF16-BE
						char *buf = new char[buflen];
						C(buflen/2)
							buf[i*2+0] = ptr[i*2+1],
							buf[i*2+1] = ptr[i*2+0];
						
						ret = fm::String::fromUtf16(buf,buf+buflen);
						
						delete[] buf;
						
						GlobalUnlock(h);
					}
				}
				
				return ret;
			}
			
			// fallback if no unicode clipdata
			if (IsClipboardFormatAvailable(CF_TEXT))
			{
				fm::String ret;
				HANDLE h = GetClipboardData(CF_TEXT);

				if (h)
				{
					ret = fm::String((LPCSTR)GlobalLock(h));

					if (ret.size())
						GlobalUnlock(h);
				}

				return ret;
			}

			return fm::String();
		}

		/////////////////////////////////////////////////////////////
		fm::Result setTextData(const fm::String &data)
		{
			std::string str = data.str();

			HANDLE h = GlobalAlloc(GMEM_MOVEABLE,(str.size()+1)*sizeof( str[0] ));

			if (!h)
				return fm::Result("MemError",fm::Result::OPFailed,"OutOfMemory","setTextData",__FILE__,__LINE__);

			char *ptr = (char*)GlobalLock(h);
			memcpy(ptr,str.c_str(),(str.size()+1)*sizeof( str[0] ));

			GlobalUnlock(h);

			SetClipboardData(CF_TEXT, h);



			std::wstring wstr = data.wstr();

			h = GlobalAlloc(GMEM_MOVEABLE,(wstr.size()+1)*sizeof( wstr[0] ));

			if (!h)
				return fm::Result("MemError",fm::Result::OPFailed,"OutOfMemory","setTextData",__FILE__,__LINE__);

			wchar_t *wptr = (wchar_t*)GlobalLock(h);
			memcpy(wptr,wstr.c_str(),(wstr.size()+1)*sizeof( wstr[0] ));

			GlobalUnlock(h);

			SetClipboardData(CF_UNICODETEXT, h);
			
			return {};
		}
	}
}
