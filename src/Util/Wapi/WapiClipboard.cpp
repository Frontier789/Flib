#include <FRONTIER/System/macros/C.hpp>
#include <FRONTIER/System/NullPtr.hpp>
#include <FRONTIER/System/FmLog.hpp>
#include <windows.h>

namespace fm
{
	void WapiPrintLastError_(Log &output,const char *functionName,const char *file, unsigned int line)
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
			// convert description to string
			ret=std::string(errorText);
			::LocalFree(errorText); // free up the allocated memory
		}

		while (ret.length() && (ret[ret.length()-1]=='\t' ||
								ret[ret.length()-1]==' '  ||
								ret[ret.length()-1]=='\r' ||
								ret[ret.length()-1]=='\n') ) ret.resize(ret.length()-1);


		output << functionName << " failed in " << file << " : " << line << " (LastError=\"" << ret << "\")=" << err << std::endl;
	}
}

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
				{
					fm::WapiPrintLastError_(fm_log,"OpenClipboard",__FILE__,__LINE__);

					return false;
				}
				return true;
			}

			/////////////////////////////////////////////////////////////
			void closeClipboard()
			{
				if (!CloseClipboard())
				{
					fm::WapiPrintLastError_(fm_log,"CloseClipboard",__FILE__,__LINE__);
				}
			}

			/////////////////////////////////////////////////////////////
			void emptyClipboard()
			{
				if (!EmptyClipboard())
				{
					fm::WapiPrintLastError_(fm_log,"EmptyClipboard",__FILE__,__LINE__);
				}
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
				{
					fm::WapiPrintLastError_(fm_log,"GlobalAlloc",__FILE__,__LINE__);
					return;
				}

				char *ptr = (char*)GlobalLock(h);
				memcpy(ptr,str.c_str(),(str.size()+1)*sizeof( str[0] ));

				GlobalUnlock(h);

				SetClipboardData(CF_TEXT, h);



				std::wstring wstr = data.wstr();

				h = GlobalAlloc(GMEM_MOVEABLE,(wstr.size()+1)*sizeof( wstr[0] ));

				if (!h)
				{
					fm::WapiPrintLastError_(fm_log,"GlobalAlloc",__FILE__,__LINE__);
					return;
				}

				wchar_t *wptr = (wchar_t*)GlobalLock(h);
				memcpy(wptr,wstr.c_str(),(wstr.size()+1)*sizeof( wstr[0] ));

				GlobalUnlock(h);

				SetClipboardData(CF_UNICODETEXT, h);
			}
		}
	}
}
