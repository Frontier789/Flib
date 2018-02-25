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
#ifndef FRONTIER_SHADERSOURCE_HPP_INCLUDED
#define FRONTIER_SHADERSOURCE_HPP_INCLUDED
#include <FRONTIER/System/util/API.h>
#define FRONTIER_SHADERSOURCE
#include <vector>
#include <string>

namespace fg
{
	/////////////////////////////////////////////////////////////
	/// @brief Class used to build a shader's source code
	///
	/// @ingroup Graphics
	///
	/////////////////////////////////////////////////////////////
	class FRONTIER_API ShaderSource
	{
	public:
		typedef std::string StringT; ///< Alias for the string type used
		struct DefineData   {StringT name; StringT value;}; ///< Data used in /\#define name value/ form 
		struct VariableData {StringT type; StringT name;};  ///< Generic variable data
		typedef VariableData UniformData; ///< Data used in /uniform type name;/ form
		typedef VariableData OutputData;  ///< Data used in /in type name;/ form
		typedef VariableData InputData;   ///< Data used in /out type name;/ form
		struct FunctionData {StringT declaration; StringT body;}; ///< Data used in 
		
		int version; ///< The shader version (like 130)
		std::vector<DefineData>   defines;   ///< Defines
		std::vector<UniformData>  uniforms;  ///< Uniforms
		std::vector<InputData>    inputs;    ///< Inputs
		std::vector<OutputData>   outputs;   ///< Outputs
		StringT                   globals;   ///< Global text
		std::vector<FunctionData> functions; ///< Functions
		StringT                   mainBody;  ///< Main bdy text
		unsigned int              type;      ///< The type of the shader
		
		/////////////////////////////////////////////////////////////
		/// @brief Class used to build a shader's source code
		///
		/// @return The generated source code
		///
		/////////////////////////////////////////////////////////////
		StringT generateSource() const;
		
		/////////////////////////////////////////////////////////////
		/// @brief String conversion operator
		///
		/////////////////////////////////////////////////////////////
		operator StringT() const;
	};
}
#endif // FRONTIER_SHADERSOURCE_INCLUDED

#ifndef FRONTIER_DONT_INCLUDE_INL
	#include <FRONTIER/Graphics/ShaderSource.inl>
#endif
