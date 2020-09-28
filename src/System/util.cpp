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
#include <FRONTIER/System/util/file_content.hpp>
#include <FRONTIER/System/util/API.h>
#include <fstream>

namespace fm
{
	fm::Result FRONTIER_API file_content(std::string file,std::string &content)
    {
        std::ifstream in(file.c_str(), std::ios::in | std::ios::binary);
        if (in)
        {
            in.seekg(0, std::ios::end);
            content.resize(in.tellg());
            in.seekg(0, std::ios::beg);
            in.read(&content[0], content.size());
            in.close();
            
            return {};
        }
        
        return fm::Result("IOError",fm::Result::OPFailed,"FileNotFound","loadFromFiles",__FILE__,__LINE__,file);
    }
}