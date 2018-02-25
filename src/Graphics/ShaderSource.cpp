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
#include <FRONTIER/Graphics/ShaderSource.hpp>
#include <FRONTIER/System/String.hpp>

namespace fg
{
	/////////////////////////////////////////////////////////////
	ShaderSource::StringT ShaderSource::generateSource() const
	{
		StringT ret;
		
		ret += "#version " + fm::toString(version).str() + "\n\n";
		
		for (auto data : defines)  { ret += "#define " + data.name + " " + data.value + "\n"; } ret+="\n";
		for (auto data : uniforms) { ret += "uniform " + data.type + " " + data.name + ";\n"; } ret+="\n";
		for (auto data : inputs)   { ret += "in "      + data.type + " " + data.name + ";\n"; } ret+="\n";
		for (auto data : outputs)  { ret += "out "     + data.type + " " + data.name + ";\n"; } ret+="\n";
		
		ret += globals + "\n";
		
		ret += "void main()\n";
		ret += "{\n";
		ret += mainBody + "\n";
		ret += "}\n";
		
		return ret;
	}
	
	/////////////////////////////////////////////////////////////
	ShaderSource::operator ShaderSource::StringT() const
	{
		return generateSource();
	}
}
