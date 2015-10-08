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
#include <windows.h>

namespace fm
{
	/////////////////////////////////////////////////////////////
	namespace util
	{
		/////////////////////////////////////////////////////////////
		namespace fileSys
		{
			namespace priv
			{
				std::deque<Entry> lsHelper(const fm::String &pattern,bool (*filter)(const Entry &) = fm::nullPtr,bool listPS = true)
				{
					fm::String cpy = pattern;
				
					C(cpy.size())
						cpy[i] = (cpy[i] == '/' ? '\\' : cpy[i]);
					
					std::wstring wstr = cpy.wstr();
					
					// if the pattern matches a directory modify it to mean everything in the dir
					DWORD ftyp = GetFileAttributesW(wstr.c_str());
					if (ftyp != INVALID_FILE_ATTRIBUTES && (ftyp & FILE_ATTRIBUTE_DIRECTORY))
					{
						if (cpy[cpy.size()-1] != '\\')
							cpy.push_back("\\*");
						else
							cpy.push_back("*");
					}
					
					wstr = cpy.wstr();
					
					fm::String dir = cpy.substr(0,cpy.find_last_of("\\"));
					std::deque<Entry> ret;
					
					// begin the search
					HANDLE hFind;
					WIN32_FIND_DATAW data;

					hFind = FindFirstFileW(wstr.c_str(), &data);
					if (hFind != INVALID_HANDLE_VALUE) 
					{
						do 
						{
							Entry desc = Entry(dir,data.cFileName,(data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY));
							
							if ((desc.name != "." && desc.name != "..") || listPS)
								if (!filter || filter(desc))
									ret.push_back(desc);
						} 
						while (FindNextFileW(hFind, &data));
						
						FindClose(hFind);
					}
					
					return ret;
				}
			}
			/////////////////////////////////////////////////////////////
			std::deque<Entry> ls(const fm::String &pattern,bool recursive,bool (*filter)(const Entry &))
			{
				std::deque<Entry> all = priv::lsHelper(pattern,filter,true);
				std::deque<Entry> tmp;
				
				if (recursive)
				{
					for (fm::Size i=0;i<all.size();++i)
						if (all[i].dir && all[i].name != "." && all[i].name != "..")
							tmp = priv::lsHelper(all[i].path+"\\"+all[i].name,filter,false),
							all.insert(all.end(),tmp.begin(),tmp.end());
				}
				
				return all;
			}
		}
	}
}