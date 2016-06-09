#include <FRONTIER/System/macros/C.hpp>
#include <FRONTIER/System/NullPtr.hpp>
#include <windows.h>

namespace fm
{
	/////////////////////////////////////////////////////////////
	namespace util
	{
		/////////////////////////////////////////////////////////////
		namespace clipboard
		{
			/////////////////////////////////////////////////////////////
			bool openClipboard()
			{
				if (!OpenClipboard(fm::nullPtr))
                    return false;

				return true;
			}

			/////////////////////////////////////////////////////////////
			void closeClipboard()
			{
				CloseClipboard();
			}

			/////////////////////////////////////////////////////////////
			void emptyClipboard()
			{
				EmptyClipboard();
			}

			/////////////////////////////////////////////////////////////
			bool hasTextData()
			{
				return IsClipboardFormatAvailable(CF_UNICODETEXT) || IsClipboardFormatAvailable(CF_TEXT);
			}

			/////////////////////////////////////////////////////////////
			fm::String getData()
			{
				if (IsClipboardFormatAvailable(CF_TEXT))
				{
					fm::String ret;
					HANDLE h = GetClipboardData(CF_TEXT);

					if (h)
					{
						ret = fm::String((LPTSTR)GlobalLock(h));

						if (ret.size())
							GlobalUnlock(h);
					}

					return ret;
				}
				if (IsClipboardFormatAvailable(CF_UNICODETEXT))
				{
					fm::String ret;
					HANDLE h = GetClipboardData(CF_UNICODETEXT);

					if (h)
					{
						ret = fm::String((LPWSTR)GlobalLock(h));

						if (ret.size())
							GlobalUnlock(h);
					}

					return ret;
				}

				return fm::String();
			}

			/////////////////////////////////////////////////////////////
			void setData(const fm::String &data)
			{
				std::string str = data.str();

				HANDLE h = GlobalAlloc(GMEM_MOVEABLE,(str.size()+1)*sizeof( str[0] ));

				if (!h)
					return;

				char *ptr = (char*)GlobalLock(h);
				memcpy(ptr,str.c_str(),(str.size()+1)*sizeof( str[0] ));

				GlobalUnlock(h);

				SetClipboardData(CF_TEXT, h);



				std::wstring wstr = data.wstr();

				h = GlobalAlloc(GMEM_MOVEABLE,(wstr.size()+1)*sizeof( wstr[0] ));

				if (!h)
					return;

				wchar_t *wptr = (wchar_t*)GlobalLock(h);
				memcpy(wptr,wstr.c_str(),(wstr.size()+1)*sizeof( wstr[0] ));

				GlobalUnlock(h);

				SetClipboardData(CF_UNICODETEXT, h);
			}
		}
	}
}
